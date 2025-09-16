// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/BoidManagerSubsystem.h"
#include "Internal Logic/Bounds/WorldBounds.h"

void UBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeBoids();
	
	WorldBounds = MakeUnique<FWorldBounds>(BOIDS_BOUNDS);
}

void UBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UBoidManagerSubsystem::PostAllActorsBeginPlay()
{
	OnBoundsUpdate.Broadcast(WorldBounds->GetBoundsCenter(), WorldBounds->GetBoundsSize());
}

void UBoidManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RealDeltaTimeSpeed = BOID_MAX_SPEED * DeltaTime;	
	UpdateBoids();
	OnBoidsUpdate.Broadcast();
}

TStatId UBoidManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTickableBoidManagerSubsystem, STATGROUP_Tickables);
}



void UBoidManagerSubsystem::InitializeBoids()
{
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		Boids.Add(MakeShared<Boid>(BOIDS_BOUNDS));
		NewCalculatedVelocityPerBoid.Add(Boids[i]->Velocity);
	}
}

void UBoidManagerSubsystem::UpdateBoids()
{
	CheckOutOfBounds();

	TestUpdateAllInOne();
	
	// UpdateAlignment();
	// UpdateCohesion();
	// UpdateSeparation();
	
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		Boids[i]->Velocity = NewCalculatedVelocityPerBoid[i];
		Boids[i]->Velocity.Normalize();
		Boids[i]->Velocity *= RealDeltaTimeSpeed;
		NewCalculatedVelocityPerBoid[i] = Boids[i]->Velocity;
	}
	
	for (auto CurrentBoid: Boids)
	{
		CurrentBoid->Update();
	}
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

TArray<TSharedPtr<Boid>> UBoidManagerSubsystem::GetNeighbourBoids(int32 BoidIndexToCheckNeighbours)
{
	TArray<TSharedPtr<Boid>> NeighbourBoids;
	CheckBoidsSubarrayForValidBoids(0, BoidIndexToCheckNeighbours, BoidIndexToCheckNeighbours, NeighbourBoids);
	CheckBoidsSubarrayForValidBoids(BoidIndexToCheckNeighbours + 1, BOIDS_COUNT, BoidIndexToCheckNeighbours, NeighbourBoids);
	
	return NeighbourBoids;
}

void UBoidManagerSubsystem::CheckBoidsSubarrayForValidBoids(int32 StartIndex, int32 EndIndex, int32 BoidIndexToCheckNeighbours, TArray<TSharedPtr<Boid>>& ValidBoids)
{
	for (int i = StartIndex; i < EndIndex; i++)
	{
		float DistanceToOtherBoid = FVector::Dist(Boids[BoidIndexToCheckNeighbours]->Position, Boids[i]->Position);

		if (DistanceToOtherBoid <= PERCEPTION_DISTANCE)
		{
			ValidBoids.Add(Boids[i]);
		}
	}
}

TArray<FVector> UBoidManagerSubsystem::GetNeighbourBoidsLocations(int32 BoidIndexToCheckNeighbours)
{
	TArray<TSharedPtr<Boid>> NeighbourBoids = GetNeighbourBoids(BoidIndexToCheckNeighbours);
	
	TArray<FVector> FinalLocations;

	for (int i = 0; i < NeighbourBoids.Num(); i++)
	{
		FinalLocations.Add(NeighbourBoids[i]->Position);
	}
	
	return FinalLocations;
}

void UBoidManagerSubsystem::TestUpdateAllInOne()
{
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		// TODO: PUT EVERYTHING INTO IT AND SEE THE PERFORMANCE LATER PROPERLY :>
		TArray<TSharedPtr<Boid>> NeighbourBoids = GetNeighbourBoids(i);
		
		if (NeighbourBoids.Num() == 0)
		{
			continue;
		}
		
		FVector SeparationVector = FVector::ZeroVector;
		FVector AlignmentVector = FVector::ZeroVector;
		FVector CohesionVector = FVector::ZeroVector;
				
		for (int j = 0; j < NeighbourBoids.Num(); j++)
		{
			float DistanceToOtherBoid = FVector::Dist(Boids[i]->Position, NeighbourBoids[j]->Position);
			
			FVector DesiredSeparationDirection = Boids[i]->Position - NeighbourBoids[j]->Position;
			DesiredSeparationDirection.Normalize();
			DesiredSeparationDirection *= 1.5 - DistanceToOtherBoid / PERCEPTION_DISTANCE;
			SeparationVector += DesiredSeparationDirection;

			FVector OtherBoidVelocity = NeighbourBoids[j]->Velocity;
			OtherBoidVelocity.Normalize();
			AlignmentVector += OtherBoidVelocity;

			CohesionVector += NeighbourBoids[j]->Position;
		}
		
		SeparationVector /= NeighbourBoids.Num();
		SeparationVector *= SEPARATION_FORCE;
		
		AlignmentVector /= NeighbourBoids.Num();
		AlignmentVector *= ALIGNMENT_FORCE;
		
		CohesionVector /= NeighbourBoids.Num();
		CohesionVector -= Boids[i]->Position;
		CohesionVector.Normalize();
		CohesionVector *= COHESION_FORCE;

		NewCalculatedVelocityPerBoid[i] += SeparationVector + AlignmentVector + CohesionVector;
	}
}

void UBoidManagerSubsystem::UpdateSeparation()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector SeparationVector = SeparationResultPerBoid(i);
		
		NewCalculatedVelocityPerBoid[i] += SeparationVector;
	}	
}

FVector UBoidManagerSubsystem::SeparationResultPerBoid(int32 BoidIndexToCalculate)
{
	FVector FinalSeparationVector = FVector::ZeroVector;
	int TotalCalculatedNeighbours = 0;
		
	for (int i = 0; i < Boids.Num(); i++)
	{
		if (BoidIndexToCalculate == i)
		{
			continue;
		}

		float DistanceToOtherBoid = FVector::Dist(Boids[BoidIndexToCalculate]->Position, Boids[i]->Position);

		if (DistanceToOtherBoid <= PERCEPTION_DISTANCE)
		{
			FVector DesiredDirection = Boids[BoidIndexToCalculate]->Position - Boids[i]->Position;
			DesiredDirection.Normalize();
			DesiredDirection *= 1.5 - DistanceToOtherBoid / PERCEPTION_DISTANCE;
			
			FinalSeparationVector += DesiredDirection;	
			TotalCalculatedNeighbours++;
		}
	}

	if (TotalCalculatedNeighbours > 0)
	{
		FinalSeparationVector /= TotalCalculatedNeighbours;
		FinalSeparationVector *= SEPARATION_FORCE;
	}

	return FinalSeparationVector;
}

void UBoidManagerSubsystem::UpdateAlignment()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector AlignmentVector = AlignmentResultPerBoid(i);

		NewCalculatedVelocityPerBoid[i] += AlignmentVector;
	}	
}

FVector UBoidManagerSubsystem::AlignmentResultPerBoid(int32 BoidIndexToCalculate)
{
	FVector FinalAlignmentVector = FVector::ZeroVector;
	int TotalCalculatedNeighbours = 0;
		
	for (int i = 0; i < Boids.Num(); i++)
	{
		if (BoidIndexToCalculate == i)
		{
			continue;
		}

		if (IsInRange(BoidIndexToCalculate, i))
		{
			FVector NormalizedBoidVelocity = Boids[i]->Velocity;
			NormalizedBoidVelocity.Normalize();
			
			FinalAlignmentVector += NormalizedBoidVelocity;			
			TotalCalculatedNeighbours++;
		}
	}

	if (TotalCalculatedNeighbours > 0)
	{
		FinalAlignmentVector /= TotalCalculatedNeighbours;
		FinalAlignmentVector *= ALIGNMENT_FORCE;
	}

	return FinalAlignmentVector;
}

void UBoidManagerSubsystem::UpdateCohesion()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector CohesionVector = CohesionResultPerBoid(i);
		
		NewCalculatedVelocityPerBoid[i] += CohesionVector;
	}	
}

FVector UBoidManagerSubsystem::CohesionResultPerBoid(int BoidIndexToCalculate)
{
	FVector FinalCohesionVector = FVector::ZeroVector;
	int TotalCalculatedNeighbours = 0;
		
	for (int i = 0; i < Boids.Num(); i++)
	{
		if (BoidIndexToCalculate == i)
		{
			continue;
		}
		
		if (IsInRange(BoidIndexToCalculate, i))
		{
			FinalCohesionVector += Boids[i]->Position;		
			TotalCalculatedNeighbours++;
		}
	}

	if (TotalCalculatedNeighbours > 0)
	{
		FinalCohesionVector /= TotalCalculatedNeighbours;
		FinalCohesionVector -= Boids[BoidIndexToCalculate]->Position;
		FinalCohesionVector.Normalize();
		FinalCohesionVector *= COHESION_FORCE;
	}

	return FinalCohesionVector;
}

bool UBoidManagerSubsystem::IsInRange(int32 FirstIndex, int32 SecondIndex)
{
	float DistanceBetweenBoids = FVector::Dist(Boids[FirstIndex]->Position, Boids[SecondIndex]->Position);
	return DistanceBetweenBoids <= PERCEPTION_DISTANCE;
}

void UBoidManagerSubsystem::CheckOutOfBounds()
{
	if (!WorldBounds.IsValid())
	{
		return;
	}
	
	for (auto CurrentBoid: Boids)
	{
		WorldBounds->WrapPosition(CurrentBoid->Position);
	}
}
