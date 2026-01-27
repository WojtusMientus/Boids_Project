
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
	
	// RemakeBoidCollisionVoxelGrid();
	
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		const FBoidsSpeciesPlainInfo& CurrentSpeciesInfo = BoidSpecies[SpeciesIndex]->SpeciesInfo;
		
		for (int BoidIndex = 0; BoidIndex < BoidSpecies[SpeciesIndex]->Num(); BoidIndex++)
		{
			FBoid& Boid = BoidSpecies[SpeciesIndex]->BoidPool[BoidIndex];
			Boid.Acceleration = FVector::ZeroVector;
			
			GetNeighbourBoidsDifferentSpeciesBruteForce(SpeciesIndex, BoidIndex, CurrentSpeciesInfo.PerceptionDistanceSquared);
			
			// {
			// 	TRACE_CPUPROFILER_EVENT_SCOPE(BoidManager_Neighbor_VoxelGrid)
			// 	GetNeighborBoidsDifferentSpeciesVoxelGrid(Boid, SpeciesIndex);
			// }

			ApplyAllBoidBehaviourForces(Boid, SpeciesIndex, BoidIndex);
			
			ApplyEnvironmentCollisionForce(Boid, CurrentSpeciesInfo.EnvironmentCollisionMultiplier, 
				CurrentSpeciesInfo.BoundsCollisionMultiplier);
			ApplySpeedAdjustmentForcePerSpecies(Boid, CurrentSpeciesInfo.DesiredSpeed);
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

void UBoidManagerSubsystem::GetNeighbourBoidsDifferentSpeciesBruteForce(const int32 SpeciesID, const int32 BoidID, 
	const float PerceptionDistanceSquared)
{
	SameSpeciesNeighbors.Reset();
	DifferentSpeciesNeighbors.Reset();
	
	HandleSameSpeciesNeighborSearch(SpeciesID, BoidID, PerceptionDistanceSquared);
	
	int32 OtherSpeciesID = (SpeciesID + 1) % BoidSpecies.Num();
	
	while (OtherSpeciesID != SpeciesID)
	{
		HandleDifferentSpeciesNeighborSearch(SpeciesID, OtherSpeciesID, BoidID, PerceptionDistanceSquared);
		OtherSpeciesID = (OtherSpeciesID + 1) % BoidSpecies.Num();
	}
}

void UBoidManagerSubsystem::HandleSameSpeciesNeighborSearch(const int32 SpeciesID, const int32 BoidID, 
                                                            const float PerceptionDistanceSquared)
{
	const FVector& CurrentBoidPositon = BoidSpecies[SpeciesID]->BoidPool[BoidID].Position;

	for (int BoidIndex = 0; BoidIndex < BoidID; BoidIndex++)
	{
		if (IsWithinPerceptionRangeDifferentSpecies(CurrentBoidPositon, 
			BoidSpecies[SpeciesID]->BoidPool[BoidIndex].Position, PerceptionDistanceSquared))
		{
			SameSpeciesNeighbors.Add(BoidIndex);
		}
	}
	
	for (int BoidIndex = BoidID + 1; BoidIndex < BoidSpecies[SpeciesID]->Num(); BoidIndex++)
	{
		if (IsWithinPerceptionRangeDifferentSpecies(CurrentBoidPositon, 
			BoidSpecies[SpeciesID]->BoidPool[BoidIndex].Position, PerceptionDistanceSquared))
		{
			SameSpeciesNeighbors.Add(BoidIndex);
		}
	}
}

void UBoidManagerSubsystem::HandleDifferentSpeciesNeighborSearch(const int32 SpeciesID, const int32 OtherSpeciesID,
                                                                 const int32 BoidID, const float PerceptionDistanceSquared)
{
	const FVector& CurrentBoidPositon = BoidSpecies[SpeciesID]->BoidPool[BoidID].Position;
	
	for (int BoidIndex = 0; BoidIndex < BoidSpecies[OtherSpeciesID]->Num(); BoidIndex++)
	{
		if (IsWithinPerceptionRangeDifferentSpecies(CurrentBoidPositon,
			BoidSpecies[OtherSpeciesID]->BoidPool[BoidIndex].Position, PerceptionDistanceSquared))
		{
			DifferentSpeciesNeighbors.Add(FBoidCollisionCellData(OtherSpeciesID, BoidIndex));
		}
	}
}

void UBoidManagerSubsystem::ApplyAllBoidBehaviourForces(FBoid& Boid, const uint8 SpeciesID, const uint16 BoidID)
{
	FVector Alignment = FVector::Zero();
	FVector Cohesion = FVector::Zero();
	FVector Separation = FVector::Zero();
	
	ComputeBoidBehaviorForces(SpeciesID, BoidID, Separation, Alignment, Cohesion);
	const FVector OtherSpeciesForce = ComputeForceBetweenDifferentSpecies(SpeciesID, BoidID);
	
	Boid.Acceleration = Separation + Alignment + Cohesion + OtherSpeciesForce;
}

void UBoidManagerSubsystem::ComputeBoidBehaviorForces(const int32 SpeciesID, const int32 BoidID, 
                                                      FVector& SeparationVector, FVector& AlignmentVector, FVector& CohesionVector)
{
	const FVector& CurrentBoidPosition = BoidSpecies[SpeciesID]->BoidPool[BoidID].Position;
	const FBoidsSpeciesPlainInfo& CurrentSpeciesInfo = BoidSpecies[SpeciesID]->SpeciesInfo;
	
	for (const int32 NeighborBoidID: SameSpeciesNeighbors)
	{
		const FBoid& NeighborBoid = BoidSpecies[SpeciesID]->BoidPool[NeighborBoidID];
		
		AddSeparationForcePerNeighbor(CurrentBoidPosition, NeighborBoid.Position, 
			CurrentSpeciesInfo.PerceptionDistance, SeparationVector);
		AddAlignmentForcePerNeighbor(NeighborBoid.Velocity, AlignmentVector);
		AddCohesionForcePerNeighbor(NeighborBoid.Position, CohesionVector);
	}
	
	if (SameSpeciesNeighbors.Num() > 0)
	{
		ComputeFinalSeparationForce(CurrentSpeciesInfo.SeparationForce, SeparationVector);
		ComputeFinalAlignmentForce(CurrentSpeciesInfo.AlignmentForce, AlignmentVector);
		ComputeFinalCohesionForce(CurrentBoidPosition, CurrentSpeciesInfo.CohesionForce, CohesionVector);
	}
}

void UBoidManagerSubsystem::AddSeparationForcePerNeighbor(const FVector& CurrentBoidPosition, const FVector& OtherBoidPosition,
	const float PerceptionDistance, FVector& SeparationVector)
{
	FVector DesiredDirection = CurrentBoidPosition - OtherBoidPosition;
	float DistanceToOtherBoid = DesiredDirection.Length();
		
	DesiredDirection = (1 - DistanceToOtherBoid / PerceptionDistance) * DesiredDirection.GetSafeNormal();
	SeparationVector += DesiredDirection;	
}

void UBoidManagerSubsystem::AddAlignmentForcePerNeighbor(const FVector& OtherBoidVelocity, FVector& AlignmentVector)
{
	FVector DesiredBoidVelocity = OtherBoidVelocity;
	DesiredBoidVelocity = DesiredBoidVelocity.GetSafeNormal();
	AlignmentVector += DesiredBoidVelocity;
}

void UBoidManagerSubsystem::AddCohesionForcePerNeighbor(const FVector& OtherBoidPosition, FVector& CohesionVector)
{
	CohesionVector += OtherBoidPosition;		
}

void UBoidManagerSubsystem::ComputeFinalSeparationForce(const float SeparationMultiplier, FVector& SeparationVector)
{
	SeparationVector /= SameSpeciesNeighbors.Num();
	SeparationVector *= SeparationMultiplier;
}

void UBoidManagerSubsystem::ComputeFinalAlignmentForce(const float AlignmentMultiplier, FVector& AlignmentVector)
{
	AlignmentVector /= SameSpeciesNeighbors.Num();
	AlignmentVector *= AlignmentMultiplier;
}

void UBoidManagerSubsystem::ComputeFinalCohesionForce(const FVector& CurrentBoidPosition, const float CohesionMultiplier,
	FVector& CohesionVector)
{
	CohesionVector /= SameSpeciesNeighbors.Num();
	CohesionVector -= CurrentBoidPosition;
	CohesionVector = CohesionVector.GetSafeNormal();
	CohesionVector *= CohesionMultiplier;
}

FVector UBoidManagerSubsystem::ComputeForceBetweenDifferentSpecies(const int32 SpeciesID, const int32 BoidID)
{
	FVector FinalOtherSpeciesVector = FVector::ZeroVector;
	const FVector& CurrentBoidPosition = BoidSpecies[SpeciesID]->BoidPool[BoidID].Position;
	
	for (const FBoidCollisionCellData OtherSpeciesData: DifferentSpeciesNeighbors)
	{
		FinalOtherSpeciesVector += (CurrentBoidPosition - 
			BoidSpecies[OtherSpeciesData.SpeciesIndex]->BoidPool[OtherSpeciesData.BoidIndex].Position).GetSafeNormal();		
	}
	
	if (DifferentSpeciesNeighbors.Num() > 0)
	{
		FinalOtherSpeciesVector /= DifferentSpeciesNeighbors.Num();
		FinalOtherSpeciesVector *= BoidSpecies[SpeciesID]->SpeciesInfo.OtherSpeciesForceMultiplier;
	}

	return FinalOtherSpeciesVector;
}

void UBoidManagerSubsystem::ApplyEnvironmentCollisionForce(FBoid& CurrentBoid, 
	const float EnvironmentCollisionMultiplier, const float BoundsCollisionMultiplier)
{
	CurrentBoid.Acceleration += WorldCollisionVoxelGrid->GetFinalCollisionVectorAtLocation(
	CurrentBoid.Position, EnvironmentCollisionMultiplier, BoundsCollisionMultiplier);
}

void UBoidManagerSubsystem::ApplySpeedAdjustmentForcePerSpecies(FBoid& CurrentBoid, const float DesiredSpeed)
{
	const FVector DesiredVelocity = CurrentBoid.Velocity.GetSafeNormal() * DesiredSpeed;
	const FVector CorrectionVector = DesiredVelocity - CurrentBoid.Velocity;
	CurrentBoid.Acceleration += CorrectionVector * SPEED_CORRECTION_FORCE;
}

void UBoidManagerSubsystem::ApplyRecalculatedVelocity(float DeltaTime)
{
	for (int SpeciesIndex = 0; SpeciesIndex < BoidSpecies.Num(); SpeciesIndex++)
	{
		const FBoidsSpeciesPlainInfo& CurrentSpeciesInfo = BoidSpecies[SpeciesIndex]->SpeciesInfo;
		
		for (int BoidIndex = 0; BoidIndex < BoidSpecies[SpeciesIndex]->Num(); BoidIndex++)
		{
			FBoid& CurrentBoid = BoidSpecies[SpeciesIndex]->BoidPool[BoidIndex];
		
			CurrentBoid.Velocity += CurrentBoid.Acceleration * DeltaTime;
			CurrentBoid.Velocity = CurrentBoid.Velocity.GetClampedToMaxSize(1.5f * CurrentSpeciesInfo.DesiredSpeed);
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
