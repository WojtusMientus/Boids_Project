
#include "Core/BoidManagerSubsystem.h"

void UBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	InitializeBoids();
	WorldCollisionBounds = MakeUnique<FWorldCollisionBounds>(BOIDS_BOUNDS);
	
	if (UWorld* World = GetWorld())
	{
		World->OnWorldBeginPlay.AddUObject(this, &UBoidManagerSubsystem::PostAllActorsBeginPlay);
	}
}

void UBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UBoidManagerSubsystem::PostAllActorsBeginPlay() const
{
	OnBoundsUpdate.Broadcast(WorldCollisionBounds->GetCenter(), WorldCollisionBounds->GetSize());
}

void UBoidManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateBoids(DeltaTime);
	OnBoidsUpdateFinish.Broadcast();
}

TStatId UBoidManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTickableBoidManagerSubsystem, STATGROUP_Tickables);
}

FVector UBoidManagerSubsystem::GetBoidPositionAt(int32 Index)
{
	if (!Boids[Index])
	{
		return FVector::ZeroVector;
	}

	return Boids[Index]->Position;
}

FVector UBoidManagerSubsystem::GetBoidVelocityAt(int32 Index)
{
	if (!Boids[Index])
	{
		return FVector::ZeroVector;
	}

	return Boids[Index]->Velocity;
}

void UBoidManagerSubsystem::InitializeBoids()
{
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		const FVector InitialPosition = CalculateBoidInitialPosition();
		const FVector InitialVelocity = FMath::VRand() * BOID_DESIRED_VELOCITY;
		Boids.Add(MakeUnique<FBoid>(InitialPosition, InitialVelocity));
		NewCalculatedVelocityPerBoid.Add(Boids[i]->Velocity);
	}
	CurrentNeighbours.Reserve(BOIDS_COUNT);
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
		if (!CurrentBoid)
		{
			continue;
		}
		
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
		if (!CurrentBoid)
		{
			continue;
		}
		
		CurrentBoid->Velocity += CurrentBoid->Acceleration * DeltaTime;
		CurrentBoid->Velocity = CurrentBoid->Velocity.GetClampedToMaxSize(2 * BOID_DESIRED_VELOCITY);
		CurrentBoid->Update(DeltaTime);
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

void UBoidManagerSubsystem::ApplySpeedAdjustmentForce(FBoid* CurrentBoid)
{
	const FVector DesiredVelocity = CurrentBoid->Velocity.GetSafeNormal() * BOID_DESIRED_VELOCITY;
	const FVector CorrectionVector = DesiredVelocity - CurrentBoid->Velocity;
	CurrentBoid->Acceleration += CorrectionVector * SPEED_CORRECTION_FORCE;
}

TArray<FVector> UBoidManagerSubsystem::GetNeighbourBoidsLocations(int32 BoidIndexToCheckNeighbours)
{
	TArray<FBoid*> NeighbourBoids;
	GetNeighbourBoids(BoidIndexToCheckNeighbours, NeighbourBoids);
	
	TArray<FVector> FinalLocations;
	for (int i = 0; i < NeighbourBoids.Num(); i++)
	{
		FinalLocations.Add(NeighbourBoids[i]->Position);
	}
	
	return FinalLocations;
}
