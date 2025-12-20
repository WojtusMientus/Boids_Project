
#include "Core/Subsystems/BoidManagerSubsystem.h"
#include "Bounds/VoxelGrid/VoxelGridData/VoxelGridData.h"
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
	
	WorldCollisionGrid = MakeUnique<FWorldCollisionVoxelGrid>();
	check(WorldCollisionGrid.IsValid())
}

void UBoidManagerSubsystem::UpdateBoidCellIndices(const int32 ID, const int32 VoxelGridIndex,
	const int32 VoxelGridCellIndex)
{
	Boids[ID]->VoxelGridIndex = VoxelGridIndex;
	Boids[ID]->VoxelGridCellIndex = VoxelGridCellIndex;
}


void UBoidManagerSubsystem::Tick(float DeltaTime)
{
	UpdateBoidsTest(DeltaTime);
	
	// if (BoidVoxelGrid.IsValid())
	// {
	// 	for (int i = 0; i < Boids.Num(); i++)
	// 	{
	// 		FBoid* Boid = Boids[i].Get();			
	// 		BoidVoxelGrid->UpdateBoid(Boid->Position, Boid->VoxelGridIndex, Boid->VoxelGridCellIndex);
	// 	}
	// }
	
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

FVector UBoidManagerSubsystem::GetBoidPositionAt(int32 Index)
{
	if (!Boids.IsValidIndex(Index) || !Boids[Index])
	{
		return FVector::ZeroVector;
	}

	return Boids[Index]->Position;
}

FVector UBoidManagerSubsystem::GetBoidVelocityAt(int32 Index)
{
	if (!Boids.IsValidIndex(Index) || !Boids[Index])
	{
		return FVector::ZeroVector;
	}

	return Boids[Index]->Velocity;
}

void UBoidManagerSubsystem::InitializeSimulation(const TArray<FBoidsPlainInfo> BoidsInfo,
	const FEnvironmentCollisionVoxelGridData VoxelGridData)
{
	WorldCollisionGrid->InitializeWorldCollisionVoxelGrid(VoxelGridData);
	InitializeBoids(BoidsInfo);
	
		
	// WorldCollisionBounds = MakeUnique<FWorldCollisionBounds>(BOIDS_BOUNDS);
	
	// FVoxelGridData<TArray<int32>> VoxelGridData;
	// VoxelGridData.BoundsData.GridResolution = FIntVector(10);
	// VoxelGridData.BoundsData.Extent = FVector(1000);
	// VoxelGridData.BoundsData.Center = FVector(0, 0, 0);
	// VoxelGridData.VoxelGridData = TArray<TArray<int32>>();
	//
	// BoidVoxelGrid = MakeUnique<FBoidCollisionVoxelGrid>(VoxelGridData);
	// if (BoidVoxelGrid.IsValid())
	// {
	// 	BoidVoxelGrid->InitializeBoidCollisionGrid(BOIDS_COUNT);
	// 	
	// 	BoidVoxelGrid->OnBoidGridIndexChanged.AddUObject(this, &UBoidManagerSubsystem::UpdateBoidCellIndices);
	// 	
	// 	for (int i = 0; i < Boids.Num(); i++)
	// 	{
	// 		FBoid* Boid = Boids[i].Get();
	// 		BoidVoxelGrid->AddBoidToVoxelGrid(i, Boid->Position);
	// 	}
	// }
}

void UBoidManagerSubsystem::InitializeBoids(const TArray<FBoidsPlainInfo> BoidsInfo)
{
	int32 ValidBoidIDs = 0;
	Boids.Reserve(BOIDS_COUNT);
	CurrentNeighbours.Reserve(BOIDS_COUNT);
	
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		const FVector InitialPosition = CalculateBoidInitialPosition();
		const FVector InitialVelocity = FMath::VRand() * BOID_DESIRED_VELOCITY;
		
		TUniquePtr<FBoid> CreatedBoid = MakeUnique<FBoid>(InitialPosition, InitialVelocity);
		
		if (CreatedBoid.IsValid())
		{
			Boids.Add(MoveTemp(CreatedBoid));
			NewCalculatedVelocityPerBoid.Add(InitialVelocity);
			ValidBoidIDs++;
		}
	}
}

void UBoidManagerSubsystem::InitializeBoidsTest(const TArray<FBoidsPlainInfo> BoidsInfo)
{
	SpeciesInfo = BoidsInfo;
	DifferentSpeciesBoids.Reserve(BoidsInfo.Num());
	NewCalculatedVelocities.Reserve(BoidsInfo.Num());
	
	int32 CumulativeNumberOfBoids = 0;
	
	for (int i = 0; i < BoidsInfo.Num(); i++)
	{
		DifferentSpeciesBoids.Add(TArray<TUniquePtr<FBoid>>());
		DifferentSpeciesBoids[i].Reserve( BoidsInfo[i].Number);
		
		NewCalculatedVelocities.Add(TArray<FVector>());
		NewCalculatedVelocities[i].Reserve( BoidsInfo[i].Number);
		
		for (int j = 0; j < BoidsInfo[i].Number; j++)
		{
			const FVector InitialPosition = WorldCollisionGrid->GetRandomPointInsideGrid();
			const FVector InitialVelocity = FMath::VRand() * BoidsInfo[i].DesiredSpeed;
			
			TUniquePtr<FBoid> CreatedBoid = MakeUnique<FBoid>(InitialPosition, InitialVelocity);
			
			if (CreatedBoid.IsValid())
			{
				DifferentSpeciesBoids[i].Add(MoveTemp(CreatedBoid));
				NewCalculatedVelocities[i].Add(InitialVelocity);
			}
		}
		
		CumulativeNumberOfBoids += DifferentSpeciesBoids[i].Num();
	}
	
	CurrentNeighbours.Reserve(CumulativeNumberOfBoids);
	OnBoidsInitializationFinish.Broadcast(BoidsInfo);
}

FVector UBoidManagerSubsystem::CalculateBoidInitialPosition()
{
	return FVector(FMath::RandRange(-BOIDS_BOUNDS / 2, BOIDS_BOUNDS / 2),
				FMath::RandRange(-BOIDS_BOUNDS / 2, BOIDS_BOUNDS / 2),
				FMath::RandRange(-BOIDS_BOUNDS / 2, BOIDS_BOUNDS / 2));
}

void UBoidManagerSubsystem::UpdateBoids(float DeltaTime)
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FBoid* CurrentBoid = Boids[i].Get();
		
		CurrentBoid->Acceleration = FVector::ZeroVector;
		GetNeighbourBoids(i, CurrentNeighbours);

		const FVector Alignment = ComputeAlignment();
		const FVector Cohesion = ComputeCohesion(CurrentBoid);
		const FVector Separation = ComputeSeparation(CurrentBoid);

		CurrentBoid->Acceleration += Separation + Alignment + Cohesion;
		ApplyCollisionForce(CurrentBoid);
		ApplySpeedAdjustmentForce(CurrentBoid);
	}
	
	for (int i = 0; i < Boids.Num(); i++)
	{
		FBoid* CurrentBoid = Boids[i].Get();
		
		CurrentBoid->Velocity += CurrentBoid->Acceleration * DeltaTime;
		CurrentBoid->Velocity = CurrentBoid->Velocity.GetClampedToMaxSize(1.5f * BOID_DESIRED_VELOCITY);
		CurrentBoid->Update(DeltaTime);
	}
}

void UBoidManagerSubsystem::UpdateBoidsTest(float DeltaTime)
{
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentSpeciesBoids.Num(); SpeciesIndex++)
	{
		for (int i = 0; i < DifferentSpeciesBoids[SpeciesIndex].Num(); i++)
		{
			FBoid* CurrentBoid = DifferentSpeciesBoids[SpeciesIndex][i].Get();
		
			CurrentBoid->Acceleration = FVector::ZeroVector;
			ApplyCollisionForceTest(CurrentBoid);
			ApplySpeedAdjustmentForceTest(CurrentBoid, SpeciesIndex);
		}
	} 
	
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentSpeciesBoids.Num(); SpeciesIndex++)
	{
		for (int i = 0; i < DifferentSpeciesBoids[SpeciesIndex].Num(); i++)
		{
			FBoid* CurrentBoid = DifferentSpeciesBoids[SpeciesIndex][i].Get();
		
			CurrentBoid->Velocity += CurrentBoid->Acceleration * DeltaTime;
			CurrentBoid->Velocity = CurrentBoid->Velocity.GetClampedToMaxSize(1.5f * SpeciesInfo[SpeciesIndex].DesiredSpeed);
			CurrentBoid->Update(DeltaTime);
		}
	} 
}

void UBoidManagerSubsystem::GetNeighbourBoids(int32 BoidIndexToCheckNeighbours, TArray<FBoid*>& ValidBoids)
{
	ValidBoids.Reset();
	CheckBoidsSubarrayForValidBoids(0, BoidIndexToCheckNeighbours, BoidIndexToCheckNeighbours, ValidBoids);
	CheckBoidsSubarrayForValidBoids(BoidIndexToCheckNeighbours + 1, BOIDS_COUNT, BoidIndexToCheckNeighbours, ValidBoids);
}

void UBoidManagerSubsystem::CheckBoidsSubarrayForValidBoids(int32 StartIndex, int32 EndIndex, int32 BoidIndexToCheckNeighbours, TArray<FBoid*>& ValidBoids)
{
	for (int i = StartIndex; i < EndIndex; i++)
	{
		if (IsWithinPerceptionRange(BoidIndexToCheckNeighbours, i))
		{
			ValidBoids.Add(Boids[i].Get());
		}
	}
}

// NOTE: These functions could be combined into a single loop for all forces
//       (Separation, Alignment, Cohesion) to reduce iterations over neighbors.
//		 For now, since this is still in early development and clarity, I keep them separate
FVector UBoidManagerSubsystem::ComputeSeparation(const FBoid* CurrentBoid)
{
	FVector FinalSeparationVector = FVector::ZeroVector;
	int32 NeighboursCount = CurrentNeighbours.Num();
		
	for (int i = 0; i < CurrentNeighbours.Num(); i++)
	{
		float DistanceToOtherBoid = FVector::Dist(CurrentBoid->Position, CurrentNeighbours[i]->Position);
		FVector DesiredDirection = CurrentBoid->Position - CurrentNeighbours[i]->Position;
		DesiredDirection = DesiredDirection.GetSafeNormal();
		DesiredDirection *= SEPARATION_FALLOFF - DistanceToOtherBoid / PERCEPTION_DISTANCE;
			
		FinalSeparationVector += DesiredDirection;	
	}

	if (NeighboursCount > 0)
	{
		FinalSeparationVector /= NeighboursCount;
		FinalSeparationVector *= SEPARATION_FORCE;
	}

	return FinalSeparationVector;
}

// NOTE: These functions could be combined into a single loop for all forces
//       (Separation, Alignment, Cohesion) to reduce iterations over neighbors.
//		 For now, since this is still in early development and clarity, I keep them separate
FVector UBoidManagerSubsystem::ComputeAlignment()
{
	FVector FinalAlignmentVector = FVector::ZeroVector;
	int32 NeighboursCount = CurrentNeighbours.Num();
	
	for (int i = 0; i < CurrentNeighbours.Num(); i++)
	{
		FVector NormalizedBoidVelocity = CurrentNeighbours[i]->Velocity.GetSafeNormal();
		FinalAlignmentVector += NormalizedBoidVelocity;			
	}

	if (NeighboursCount > 0)
	{
		FinalAlignmentVector /= NeighboursCount;
		FinalAlignmentVector *= ALIGNMENT_FORCE;
	}

	return FinalAlignmentVector;
}

// NOTE: These functions could be combined into a single loop for all forces
//       (Separation, Alignment, Cohesion) to reduce iterations over neighbors.
//		 For now, since this is still in early development and clarity, I keep them separate
FVector UBoidManagerSubsystem::ComputeCohesion(const FBoid* CurrentBoid)
{
	FVector FinalCohesionVector = FVector::ZeroVector;
	int32 NeighboursCount = CurrentNeighbours.Num();
		
	for (int i = 0; i < CurrentNeighbours.Num(); i++)
	{
		FinalCohesionVector += CurrentNeighbours[i]->Position;		
	}

	if (NeighboursCount > 0)
	{
		FinalCohesionVector /= NeighboursCount;
		FinalCohesionVector -= CurrentBoid->Position;
		FinalCohesionVector = FinalCohesionVector.GetSafeNormal();
		FinalCohesionVector *= COHESION_FORCE;
	}

	return FinalCohesionVector;
}

bool UBoidManagerSubsystem::IsWithinPerceptionRange(int32 FirstIndex, int32 SecondIndex)
{
	float DistanceBetweenBoids = FVector::DistSquared(Boids[FirstIndex]->Position, Boids[SecondIndex]->Position);
	return DistanceBetweenBoids <= PERCEPTION_DISTANCE_SQUARED;
}

void UBoidManagerSubsystem::ApplyCollisionForce(FBoid* CurrentBoid)
{
	CurrentBoid->Acceleration += WorldCollisionBounds->GetCollisionForceAt(CurrentBoid->Position);
}

void UBoidManagerSubsystem::ApplyCollisionForceTest(FBoid* CurrentBoid)
{
	CurrentBoid->Acceleration += WorldCollisionGrid->GetFinalCollisionVectorAtLocation(CurrentBoid->Position);
}

void UBoidManagerSubsystem::ApplySpeedAdjustmentForce(FBoid* CurrentBoid)
{
	const FVector DesiredVelocity = CurrentBoid->Velocity.GetSafeNormal() * BOID_DESIRED_VELOCITY;
	const FVector CorrectionVector = DesiredVelocity - CurrentBoid->Velocity;
	CurrentBoid->Acceleration += CorrectionVector * SPEED_CORRECTION_FORCE;
}

void UBoidManagerSubsystem::ApplySpeedAdjustmentForceTest(FBoid* CurrentBoid, int SpeciesIndex)
{
	const FVector DesiredVelocity = CurrentBoid->Velocity.GetSafeNormal() * SpeciesInfo[SpeciesIndex].DesiredSpeed;
	const FVector CorrectionVector = DesiredVelocity - CurrentBoid->Velocity;
	CurrentBoid->Acceleration += CorrectionVector * SPEED_CORRECTION_FORCE;
}

