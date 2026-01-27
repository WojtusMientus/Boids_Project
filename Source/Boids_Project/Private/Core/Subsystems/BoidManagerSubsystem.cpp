
#include "Core/Subsystems/BoidManagerSubsystem.h"
#include "VoxelGrids/VoxelGridData/BoidCollisionCellData.h"
#include "VoxelGrids/VoxelGridData/VoxelGridData.h"
#include "Core/BoidDelegates.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Utilities/Subsystems/BoidDataManagerSubsystem.h"


void UBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World))
	ENSURE_ALWAYS_RETURN(World->IsGameWorld())
	
	Super::Initialize(Collection);
	Collection.InitializeDependency(UBoidDataManagerSubsystem::StaticClass());
	
	UGameInstance* GameInstance = World->GetGameInstance();
	ENSURE_ALWAYS_RETURN(IsValid(GameInstance))
	
	BoidDataManager = GameInstance->GetSubsystem<UBoidDataManagerSubsystem>();
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	BoidDataManager->OnSimulationDataLoaded.AddUObject(this, &UBoidManagerSubsystem::InitializeSimulation);
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UBoidManagerSubsystem::InitializeSimulationData);
	
	CreateVoxelGrids();
	
#if WITH_EDITOR
	SubscribeToGlobalEditorDelegates();
#endif
}

void UBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
#if WITH_EDITOR
	UnsubscribeFromGlobalEditorDelegates();
#endif
}

void UBoidManagerSubsystem::Tick(float DeltaTime)
{
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(BoidManager_Update)
		UpdateBoids(DeltaTime);	
	}
	OnBoidsUpdateFinish.Broadcast();
}

TStatId UBoidManagerSubsystem::GetStatId() const
{
	return TStatId();
}

bool UBoidManagerSubsystem::IsTickable() const
{
	if (!GetWorld())
	{
		return false;
	}
	
	return !GetWorld()->IsPaused();
}

void UBoidManagerSubsystem::CreateVoxelGrids()
{
	WorldCollisionVoxelGrid = MakeUnique<FWorldCollisionVoxelGrid>();
	check(WorldCollisionVoxelGrid.IsValid())
	
	BoidCollisionVoxelGrid = MakeUnique<FBoidCollisionVoxelGrid>();
	check(BoidCollisionVoxelGrid.IsValid())
}

void UBoidManagerSubsystem::InitializeSimulationData(UWorld* World, FWorldInitializationValues WorldInitializationValues)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	BoidDataManager->LoadSimulationDataAssets();
}

void UBoidManagerSubsystem::InitializeSimulation(const TArray<FBoidsSpeciesPlainInfo>& BoidsInfo,
	const FEnvironmentCollisionVoxelGridData& EnvironmentCollisionVoxelGridData, 
	const FBoidCollisionVoxelGridData& BoidCollisionVoxelGridData)
{
	WorldCollisionVoxelGrid->InitializeWorldCollisionVoxelGrid(EnvironmentCollisionVoxelGridData);
	InitializeDifferentBoidSpecies(BoidsInfo);
	
	InitializeBoidCollisionVoxelGrid(BoidCollisionVoxelGridData);
	InitializeNeighbourArray();
	
	InitializeRemainingBoidSpeciesData();
}

void UBoidManagerSubsystem::InitializeDifferentBoidSpecies(const TArray<FBoidsSpeciesPlainInfo>& BoidsInfo)
{
	BoidSpecies.Reserve(BoidsInfo.Num());
	
	int32 CumulativeNumberOfBoids = 0;
	
	for (int SpeciesIndex = 0; SpeciesIndex < BoidsInfo.Num(); SpeciesIndex++)
	{
		BoidSpecies.Add(MakeUnique<FBoidSpecies>(BoidsInfo[SpeciesIndex]));
		check(BoidSpecies[SpeciesIndex]);
		InitializeBoidSpecies(SpeciesIndex);
		
		CumulativeNumberOfBoids += BoidSpecies[SpeciesIndex]->BoidPool.Num();
	}
	
	SameSpeciesNeighbors.Reserve(CumulativeNumberOfBoids);
	DifferentSpeciesNeighbors.Reserve(CumulativeNumberOfBoids);
	OnBoidsInitializationFinish.Broadcast();
}

void UBoidManagerSubsystem::InitializeBoidSpecies(const int32 SpeciesID)
{
	for (int BoidIndex = 0; BoidIndex < BoidSpecies[SpeciesID]->Num(); BoidIndex++)
	{
		InitializeBoidObject(SpeciesID, BoidIndex);
	}	
}

void UBoidManagerSubsystem::InitializeBoidObject(const int32 SpeciesID, const int32 BoidID)
{
	const FVector InitialPosition = WorldCollisionVoxelGrid->GetRandomPointInsideGrid();
	const FVector InitialVelocity = FMath::VRand() * BoidSpecies[SpeciesID]->SpeciesInfo.DesiredSpeed;
	BoidSpecies[SpeciesID]->BoidPool[BoidID].Initialize(InitialPosition, InitialVelocity);
}

void UBoidManagerSubsystem::InitializeRemainingBoidSpeciesData()
{
	const FVector CellSize = BoidCollisionVoxelGrid->GetVoxelCellSize();
	
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		const float PerceptionDistance = BoidSpecies[SpeciesIndex]->SpeciesInfo.PerceptionDistance;
		
		BoidSpecies[SpeciesIndex]->SpeciesInfo.PerceptionDistanceInVoxelCellCount = FIntVector(
			FMath::CeilToInt(PerceptionDistance / CellSize.X),
			FMath::CeilToInt(PerceptionDistance / CellSize.Y),
			FMath::CeilToInt(PerceptionDistance / CellSize.Z));
		
		BoidSpecies[SpeciesIndex]->SpeciesInfo.PerceptionDistanceSquared = PerceptionDistance * PerceptionDistance;
	}
}

void UBoidManagerSubsystem::InitializeBoidCollisionVoxelGrid(const FBoidCollisionVoxelGridData& BoidCollisionVoxelGridData)
{
	int32 CumulativeNumberOfBoids = 0;
	
	for (int i = 0; i < BoidSpecies.Num(); i++)
	{
		CumulativeNumberOfBoids += BoidSpecies[i]->Num(); 
	}
	
	BoidCollisionVoxelGrid->InitializeBoidDataCollisionGrid(BoidCollisionVoxelGridData.BoidGridData, 
		CumulativeNumberOfBoids);
	BoidCollisionVoxelGrid->ResetVoxelGrid();
}

void UBoidManagerSubsystem::InitializeNeighbourArray()
{
	NeighborsBoidData.SetNumZeroed(BoidSpecies.Num());
	
	int CumulativeNumberOfBoids = 0;
	
	for (int i = 0; i < BoidSpecies.Num(); i++)
	{
		NeighborsBoidData[i].Reserve(BoidSpecies[i]->Num());
		CumulativeNumberOfBoids += BoidSpecies[i]->Num();
	}
}

void UBoidManagerSubsystem::UpdateBoids(float DeltaTime)
{
	/*
#if WITH_EDITOR
	CheckForAnyBoidNumberUpdate();
#endif
	*/
	
	RemakeBoidCollisionVoxelGrid();
	
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		for (int BoidIndex = 0; BoidIndex < BoidSpecies[SpeciesIndex]->Num(); BoidIndex++)
		{
			FBoid& Boid = BoidSpecies[SpeciesIndex]->BoidPool[BoidIndex];
			Boid.Acceleration = FVector::ZeroVector;
			GetNeighbourBoidsDifferentSpeciesSlow(SpeciesIndex, BoidIndex);

			{
				TRACE_CPUPROFILER_EVENT_SCOPE(BoidManager_Neighbor_VoxelGrid_ContinuousIndexing)
				GetNeighborBoidsDifferentSpeciesVoxelGrid(Boid, SpeciesIndex);
			}
			
			ComputeBoidBehaviourForces(Boid, SpeciesIndex, BoidIndex);
			ApplyEnvironmentCollisionForce(SpeciesIndex, BoidIndex);
			ApplySpeedAdjustmentForcePerSpecies(SpeciesIndex, BoidIndex);
		}
	} 
	
	ApplyRecalculatedVelocity(DeltaTime);
}

void UBoidManagerSubsystem::RemakeBoidCollisionVoxelGrid()
{
	BoidCollisionVoxelGrid->ResetVoxelGrid();
	
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		for (int BoidIndex = 0; BoidIndex < BoidSpecies[SpeciesIndex]->Num(); BoidIndex++)
		{
			BoidCollisionVoxelGrid->AddBoidToVoxelGrid(
				BoidSpecies[SpeciesIndex]->BoidPool[BoidIndex], SpeciesIndex, BoidIndex);
		}
	}
}

void UBoidManagerSubsystem::GetNeighborBoidsDifferentSpeciesVoxelGrid(const FBoid& Boid, const int SpeciesID)
{
	for (int i = 0; i < NeighborsBoidData.Num(); i++)
	{
		NeighborsBoidData[i].Reset();
	}
	
	BoidCollisionVoxelGrid->GetNeighboringBoidInVoxelGrid(Boid,
		BoidSpecies[SpeciesID]->SpeciesInfo.PerceptionDistanceInVoxelCellCount, NeighborsBoidData);
}

void UBoidManagerSubsystem::GetNeighbourBoidsDifferentSpeciesSlow(const int32 SpeciesID, const int32 BoidID)
{
	SameSpeciesNeighbors.Reset();
	DifferentSpeciesNeighbors.Reset();
	
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		if (SpeciesIndex == SpeciesID)
		{
			HandleSameSpeciesNeighborSearch(SpeciesID, BoidID);
		}
		else
		{
			HandleDifferentSpeciesNeighborSearch(SpeciesID, SpeciesIndex, BoidID);
		}
	}
}

void UBoidManagerSubsystem::HandleSameSpeciesNeighborSearch(const int32 SpeciesID, const int32 BoidID)
{
	for (int BoidIndex = 0; BoidIndex < BoidID; BoidIndex++)
	{
		if (IsWithinPerceptionRangeDifferentSpecies(SpeciesID, BoidID, 
			SpeciesID, BoidIndex, BoidSpecies[SpeciesID]->SpeciesInfo.PerceptionDistanceSquared))
		{
			SameSpeciesNeighbors.Add(BoidIndex);
		}
	}
	
	for (int BoidIndex = BoidID + 1; BoidIndex < BoidSpecies[SpeciesID]->Num(); BoidIndex++)
	{
		if (IsWithinPerceptionRangeDifferentSpecies(SpeciesID, BoidID, 
	SpeciesID, BoidIndex, BoidSpecies[SpeciesID]->SpeciesInfo.PerceptionDistanceSquared))
		{
			SameSpeciesNeighbors.Add(BoidIndex);
		}
	}
}

void UBoidManagerSubsystem::HandleDifferentSpeciesNeighborSearch(const int32 SpeciesID, const int32 OtherSpeciesID,
	const int32 BoidID)
{
	for (int BoidIndex = 0; BoidIndex < BoidSpecies[OtherSpeciesID]->Num(); BoidIndex++)
	{
		if (IsWithinPerceptionRangeDifferentSpecies(SpeciesID, BoidID, 
OtherSpeciesID, BoidIndex, BoidSpecies[SpeciesID]->SpeciesInfo.PerceptionDistanceSquared))
		{
			DifferentSpeciesNeighbors.Add(FBoidCollisionCellData(OtherSpeciesID, BoidIndex));
		}
	}
}

void UBoidManagerSubsystem::ComputeBoidBehaviourForces(FBoid& Boid, const uint8 SpeciesID,
	const uint16 BoidID)
{
	const FVector Alignment = ComputeAlignmentMultipleSpecies(SpeciesID);
	const FVector Cohesion = ComputeCohesionMultipleSpecies(SpeciesID, BoidID);
	const FVector Separation = ComputeSeparationMultipleSpecies(SpeciesID, BoidID);
	const FVector OtherSpeciesForce = ComputeForceBetweenDifferentSpecies(SpeciesID, BoidID);

	Boid.Acceleration = Separation + Alignment + Cohesion + OtherSpeciesForce;
}

// NOTE: These functions could be combined into a single loop for all forces
//       (Separation, Alignment, Cohesion) to reduce iterations over neighbors.
//		 But for the sake of clarity, I keep them separate
FVector UBoidManagerSubsystem::ComputeSeparationMultipleSpecies(const int32 SpeciesID, const int32 BoidID)
{
	FVector FinalSeparationVector = FVector::ZeroVector;
		
	for (const int32 NeighborBoidID: SameSpeciesNeighbors)
	{
		float DistanceToOtherBoid = FVector::Dist(BoidSpecies[SpeciesID]->BoidPool[BoidID].Position,
			BoidSpecies[SpeciesID]->BoidPool[NeighborBoidID].Position);
		FVector DesiredDirection = BoidSpecies[SpeciesID]->BoidPool[BoidID].Position - 
			BoidSpecies[SpeciesID]->BoidPool[NeighborBoidID].Position;
		
		DesiredDirection = (1 - DistanceToOtherBoid / BoidSpecies[SpeciesID]->SpeciesInfo.PerceptionDistance)
		* DesiredDirection.GetSafeNormal();
		FinalSeparationVector += DesiredDirection;	
	}

	if (SameSpeciesNeighbors.Num() > 0)
	{
		FinalSeparationVector /= SameSpeciesNeighbors.Num();
		FinalSeparationVector *= BoidSpecies[SpeciesID]->SpeciesInfo.SeparationForce;
	}

	return FinalSeparationVector;
}

// NOTE: These functions could be combined into a single loop for all forces
//       (Separation, Alignment, Cohesion) to reduce iterations over neighbors.
//		 But for the sake of clarity, I keep them separate
FVector UBoidManagerSubsystem::ComputeAlignmentMultipleSpecies(const int32 SpeciesID)
{
	FVector FinalAlignmentVector = FVector::ZeroVector;	
	
	for (const int32 NeighborBoidID: SameSpeciesNeighbors)
	{
		FVector DesiredBoidVelocity = BoidSpecies[SpeciesID]->BoidPool[NeighborBoidID].Velocity;
		DesiredBoidVelocity = DesiredBoidVelocity.GetSafeNormal();
		FinalAlignmentVector += DesiredBoidVelocity;			
	}
	
	if (SameSpeciesNeighbors.Num() > 0)
	{
		FinalAlignmentVector /= SameSpeciesNeighbors.Num();
		FinalAlignmentVector *= BoidSpecies[SpeciesID]->SpeciesInfo.AlignmentForce;
	}

	return FinalAlignmentVector;
}

// NOTE: These functions could be combined into a single loop for all forces
//       (Separation, Alignment, Cohesion) to reduce iterations over neighbors.
//		 But for the sake of clarity, I keep them separate
FVector UBoidManagerSubsystem::ComputeCohesionMultipleSpecies(const int32 SpeciesID, const int32 BoidID)
{
	FVector FinalCohesionVector = FVector::ZeroVector;
		
	for (const int32 NeighborBoidID: SameSpeciesNeighbors)
	{
		FinalCohesionVector += BoidSpecies[SpeciesID]->BoidPool[NeighborBoidID].Position;		
	}

	if (SameSpeciesNeighbors.Num() > 0)
	{
		FinalCohesionVector /= SameSpeciesNeighbors.Num();
		FinalCohesionVector -= BoidSpecies[SpeciesID]->BoidPool[BoidID].Position;
		FinalCohesionVector = FinalCohesionVector.GetSafeNormal();
		FinalCohesionVector *= BoidSpecies[SpeciesID]->SpeciesInfo.CohesionForce;
	}

	return FinalCohesionVector;
}

FVector UBoidManagerSubsystem::ComputeForceBetweenDifferentSpecies(const int32 SpeciesID, const int32 BoidID)
{
	FVector FinalOtherSpeciesVector = FVector::ZeroVector;
		
	for (const FBoidCollisionCellData OtherSpeciesData: DifferentSpeciesNeighbors)
	{
		FinalOtherSpeciesVector += (BoidSpecies[SpeciesID]->BoidPool[BoidID].Position - 
			BoidSpecies[OtherSpeciesData.SpeciesIndex]->BoidPool[OtherSpeciesData.BoidIndex].Position).GetSafeNormal();		
	}
	
	if (DifferentSpeciesNeighbors.Num() > 0)
	{
		FinalOtherSpeciesVector /= DifferentSpeciesNeighbors.Num();
		FinalOtherSpeciesVector *= BoidSpecies[SpeciesID]->SpeciesInfo.OtherSpeciesForceMultiplier;
	}
	
	return FinalOtherSpeciesVector;
}

bool UBoidManagerSubsystem::IsWithinPerceptionRangeDifferentSpecies(const int32 CallerSpeciesID,
	const int32 CalledID, const int32 NeighborSpeciesID, const int32 NeighborID, const float DistanceSquared)
{
	float DistanceBetweenBoids = FVector::DistSquared(BoidSpecies[CallerSpeciesID]->BoidPool[CalledID].Position,
	BoidSpecies[NeighborSpeciesID]->BoidPool[NeighborID].Position);
	return DistanceBetweenBoids <= DistanceSquared;
}

void UBoidManagerSubsystem::ApplyEnvironmentCollisionForce(const int32 SpeciesID, const int32 BoidID)
{
	BoidSpecies[SpeciesID]->BoidPool[BoidID].Acceleration += WorldCollisionVoxelGrid->GetFinalCollisionVectorAtLocation(
	BoidSpecies[SpeciesID]->BoidPool[BoidID].Position, BoidSpecies[SpeciesID]->SpeciesInfo.EnvironmentCollisionMultiplier, 
	 BoidSpecies[SpeciesID]->SpeciesInfo.BoundsCollisionMultiplier);
}

void UBoidManagerSubsystem::ApplySpeedAdjustmentForcePerSpecies(const int32 SpeciesID, const int32 BoidID)
{
	const FVector DesiredVelocity = BoidSpecies[SpeciesID]->BoidPool[BoidID].Velocity.GetSafeNormal() * 
	BoidSpecies[SpeciesID]->SpeciesInfo.DesiredSpeed;
	const FVector CorrectionVector = DesiredVelocity - BoidSpecies[SpeciesID]->BoidPool[BoidID].Velocity;
	BoidSpecies[SpeciesID]->BoidPool[BoidID].Acceleration += CorrectionVector * SPEED_CORRECTION_FORCE;
}

void UBoidManagerSubsystem::ApplyRecalculatedVelocity(float DeltaTime)
{
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		for (int BoidIndex = 0; BoidIndex < BoidSpecies[SpeciesIndex]->Num(); BoidIndex++)
		{
			FBoid& CurrentBoid = BoidSpecies[SpeciesIndex]->BoidPool[BoidIndex];
		
			CurrentBoid.Velocity += CurrentBoid.Acceleration * DeltaTime;
			CurrentBoid.Velocity = CurrentBoid.Velocity.GetClampedToMaxSize(1.5f * BoidSpecies[SpeciesIndex]->SpeciesInfo.DesiredSpeed);
			CurrentBoid.Update(DeltaTime);
		}
	}
}


#if WITH_EDITOR

void UBoidManagerSubsystem::CheckForAnyBoidNumberUpdate()
{
	if (BoidNumberUpdateQueue.IsEmpty())
	{
		return;
	}
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	while(!BoidNumberUpdateQueue.IsEmpty())
	{
		FBoidNumberUpdateInfo BoidNumberUpdateInfo;
		BoidNumberUpdateQueue.Dequeue(BoidNumberUpdateInfo);
		
		const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(BoidNumberUpdateInfo.Type);
		
		if (BoidNumberUpdateInfo.Count > 0)
		{
			HandleBoidAddition(MappedIndexFromGameplayTag, BoidNumberUpdateInfo.Count);
		}
		else
		{
			BoidSpecies[MappedIndexFromGameplayTag]->RemoveBoids(-BoidNumberUpdateInfo.Count);
		}
	}
	
	OnBoidsNumberUpdate.Broadcast();
}

void UBoidManagerSubsystem::HandleBoidAddition(const int32 SpeciesID, const int32 CountToAdd)
{
	const int32 NumberOfBoidsBeforeAddition = BoidSpecies[SpeciesID]->Num();
	BoidSpecies[SpeciesID]->AddBoids(CountToAdd);
	
	for (int BoidIndex = NumberOfBoidsBeforeAddition; BoidIndex < BoidSpecies[SpeciesID]->Num(); BoidIndex++)
	{
		InitializeBoidObject(SpeciesID, BoidIndex);
	}
}


void UBoidManagerSubsystem::SubscribeToGlobalEditorDelegates()
{
	OnBoidNumberUpdateHandle = BoidsDelegates::OnBoidNumberUpdate.AddUObject(this, 
		&UBoidManagerSubsystem::HandleBoidNumberUpdateHandle);
	
	OnBoidForceParametersChangeHandle = BoidsDelegates::OnBoidForceParametersChange.AddUObject(this,  
		&UBoidManagerSubsystem::HandleBoidsForceParametersChange);
	
	OnBoidSpatialAwarenessParametersChangeHandle = BoidsDelegates::OnBoidSpatialAwarenessParametersChange.AddUObject(
		this, &UBoidManagerSubsystem::HandleBoidsSpatialAwarenessParametersChange);
	
	OnBoidCollisionMultiplierUpdateHandle = BoidsDelegates::OnBoidCollisionMultiplierChange.AddUObject(this, 
		&UBoidManagerSubsystem::HandleBoidCollisionMultiplierChange);
}

void UBoidManagerSubsystem::UnsubscribeFromGlobalEditorDelegates()
{
	BoidsDelegates::OnBoidForceParametersChange.Remove(OnBoidForceParametersChangeHandle);
	BoidsDelegates::OnBoidSpatialAwarenessParametersChange.Remove(OnBoidSpatialAwarenessParametersChangeHandle);
	BoidsDelegates::OnBoidCollisionMultiplierChange.Remove(OnBoidCollisionMultiplierUpdateHandle);
}

void UBoidManagerSubsystem::HandleBoidNumberUpdateHandle(const FBoidNumberUpdateInfo& InBoidNumberUpdateInfo)
{
	BoidNumberUpdateQueue.Enqueue(InBoidNumberUpdateInfo);
}

void UBoidManagerSubsystem::HandleBoidsForceParametersChange(const FGameplayTag Tag, float NewSeparationForce,
                                                             float NewAlignmentForce, float NewCohesionForce, float NewOtherSpeciesMultiplier)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(Tag);
	
	BoidSpecies[MappedIndexFromGameplayTag]->SpeciesInfo.OverwriteForcesData(NewSeparationForce, NewAlignmentForce, 
		NewCohesionForce, NewOtherSpeciesMultiplier);
}

void UBoidManagerSubsystem::HandleBoidsSpatialAwarenessParametersChange(FGameplayTag Tag, float NewDesiredSpeed,
	float NewPerceptionDistance)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(Tag);
	
	BoidSpecies[MappedIndexFromGameplayTag]->SpeciesInfo.OverwriteSpatialAwarenessData(NewDesiredSpeed, 
		NewPerceptionDistance);
}

void UBoidManagerSubsystem::HandleBoidCollisionMultiplierChange(FGameplayTag Tag, float NewEnvironmentCollisionMultiplier,
	float NewBoundsCollisionMultiplier)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(Tag);
	
	BoidSpecies[MappedIndexFromGameplayTag]->SpeciesInfo.OverwriteCollisionMultiplierData(NewEnvironmentCollisionMultiplier, 
		NewBoundsCollisionMultiplier);
}

#endif
