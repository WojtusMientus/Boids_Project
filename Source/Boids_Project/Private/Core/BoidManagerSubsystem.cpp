
#include "Core/BoidManagerSubsystem.h"
#include "Bounds/WorldBounds.h"

void UBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	InitializeBoids();
	WorldCollisionBounds = MakeUnique<FWorldCollisionBounds>(BOIDS_BOUNDS);
}

void UBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UBoidManagerSubsystem::PostAllActorsBeginPlay()
{
	OnBoundsUpdate.Broadcast(WorldCollisionBounds->GetCenter(), WorldCollisionBounds->GetSize());
}

void UBoidManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateBoids(DeltaTime);
	OnBoidsUpdate.Broadcast();
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
		Boids.Add(MakeUnique<Boid>(BOIDS_BOUNDS));
		NewCalculatedVelocityPerBoid.Add(Boids[i]->Velocity);
	}
	CurrentNeighbours.Reserve(BOIDS_COUNT);
}

void UBoidManagerSubsystem::UpdateBoids(float DeltaTime)
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		Boid* CurrentBoid = Boids[i].Get();
		if (!CurrentBoid)
		{
			continue;
		}
		
		GetNeighbourBoids(i, CurrentNeighbours);

		FVector Alignment = ComputeAlignment();
		FVector Cohesion = ComputeCohesion(CurrentBoid);
		FVector Separation = ComputeSeparation(CurrentBoid);

		NewCalculatedVelocityPerBoid[i] += Separation + Alignment + Cohesion;
		ApplyCollisionForce(i);
	}
	
	for (int i = 0; i < Boids.Num(); i++)
	{
		Boids[i]->Velocity = NewCalculatedVelocityPerBoid[i];
		Boids[i]->Update(DeltaTime, BOID_MAX_SPEED);
		NewCalculatedVelocityPerBoid[i] = Boids[i]->Velocity;
	}
}

void UBoidManagerSubsystem::GetNeighbourBoids(int32 BoidIndexToCheckNeighbours, TArray<Boid*>& ValidBoids)
{
	ValidBoids.Reset();
	CheckBoidsSubarrayForValidBoids(0, BoidIndexToCheckNeighbours, BoidIndexToCheckNeighbours, ValidBoids);
	CheckBoidsSubarrayForValidBoids(BoidIndexToCheckNeighbours + 1, BOIDS_COUNT, BoidIndexToCheckNeighbours, ValidBoids);
}

void UBoidManagerSubsystem::CheckBoidsSubarrayForValidBoids(int32 StartIndex, int32 EndIndex, int32 BoidIndexToCheckNeighbours, TArray<Boid*>& ValidBoids)
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
FVector UBoidManagerSubsystem::ComputeSeparation(const Boid* CurrentBoid)
{
	FVector FinalSeparationVector = FVector::ZeroVector;
	int32 NeighboursCount = CurrentNeighbours.Num();
		
	for (int i = 0; i < CurrentNeighbours.Num(); i++)
	{
		float DistanceToOtherBoid = FVector::Dist(CurrentBoid->Position, CurrentNeighbours[i]->Position);
		FVector DesiredDirection = CurrentBoid->Position - CurrentNeighbours[i]->Position;
		DesiredDirection.Normalize();
		DesiredDirection *= SEPARATION_DESIRED_DIRECTION_MULTIPLIER - DistanceToOtherBoid / PERCEPTION_DISTANCE;
			
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
		FVector NormalizedBoidVelocity = CurrentNeighbours[i]->Velocity;
		NormalizedBoidVelocity.Normalize();
			
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
FVector UBoidManagerSubsystem::ComputeCohesion(const Boid* CurrentBoid)
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
		FinalCohesionVector.Normalize();
		FinalCohesionVector *= COHESION_FORCE;
	}

	return FinalCohesionVector;
}

bool UBoidManagerSubsystem::IsWithinPerceptionRange(int32 FirstIndex, int32 SecondIndex)
{
	float DistanceBetweenBoids = FVector::DistSquared(Boids[FirstIndex]->Position, Boids[SecondIndex]->Position);
	return DistanceBetweenBoids <= PERCEPTION_DISTANCE_SQUARED;
}

void UBoidManagerSubsystem::ApplyCollisionForce(int32 BoidIndex)
{
	NewCalculatedVelocityPerBoid[BoidIndex] += WorldCollisionBounds->GetCollisionForceAt(Boids[BoidIndex]->Position);
}

TArray<FVector> UBoidManagerSubsystem::GetNeighbourBoidsLocations(int32 BoidIndexToCheckNeighbours)
{
	TArray<Boid*> NeighbourBoids;
	GetNeighbourBoids(BoidIndexToCheckNeighbours, NeighbourBoids);
	
	TArray<FVector> FinalLocations;
	for (int i = 0; i < NeighbourBoids.Num(); i++)
	{
		FinalLocations.Add(NeighbourBoids[i]->Position);
	}
	
	return FinalLocations;
}