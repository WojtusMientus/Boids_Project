// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/BoidsManager.h"
#include "Internal Logic/Boid.h"

BoidsManager::BoidsManager()
{
}

BoidsManager::~BoidsManager()
{
}

void BoidsManager::InitializeBoids()
{
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		Boids.Add(MakeShared<Boid>(BOIDS_BOUNDS));
		NewCalculatedVelocityPerBoid.Add(Boids[i]->Velocity);
	}
}

void BoidsManager::UpdateBoids()
{
	CheckOutOfBounds();

	TestUpdateAllInOne();
	
	// UpdateAlignment();
	// UpdateCohesion();
	// UpdateSeparation();
	
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		Boids[i]->Velocity = NewCalculatedVelocityPerBoid[i];
		
		if (Boids[i]->Velocity.Size() > BOID_MAX_SPEED)
		{
			Boids[i]->Velocity.Normalize();
			Boids[i]->Velocity *= BOID_MAX_SPEED;
		}
		
		NewCalculatedVelocityPerBoid[i] = Boids[i]->Velocity;
	}
	
	for (auto CurrentBoid: Boids)
	{
		CurrentBoid->Update();
	}
}

FVector BoidsManager::GetBoidPositionAt(int Index)
{
	if (!Boids[Index])
	{
		return FVector::ZeroVector;
	}

	return Boids[Index]->Position;
}

FVector BoidsManager::GetBoidVelocityAt(int Index)
{
	if (!Boids[Index])
	{
		return FVector::ZeroVector;
	}

	return Boids[Index]->Velocity;
}

TArray<TSharedPtr<Boid>> BoidsManager::GetNeighbourBoids(int BoidIndexToCheckNeighbours)
{
	TArray<TSharedPtr<Boid>> NeighbourBoids;
	CheckBoidsSubarrayForValidBoids(0, BoidIndexToCheckNeighbours, BoidIndexToCheckNeighbours, NeighbourBoids);
	CheckBoidsSubarrayForValidBoids(BoidIndexToCheckNeighbours + 1, BOIDS_COUNT, BoidIndexToCheckNeighbours, NeighbourBoids);
	
	return NeighbourBoids;
}

void BoidsManager::CheckBoidsSubarrayForValidBoids(int StartIndex, int EndIndex, int BoidIndexToCheckNeighbours, TArray<TSharedPtr<Boid>>& ValidBoids)
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

void BoidsManager::TestUpdateAllInOne()
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

void BoidsManager::UpdateSeparation()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector SeparationVector = SeparationResultPerBoid(i);
		
		NewCalculatedVelocityPerBoid[i] += SeparationVector;
		// Boids[i]->Velocity += SeparationVector;
	}	
}

FVector BoidsManager::SeparationResultPerBoid(int BoidIndexToCalculate)
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

void BoidsManager::UpdateAlignment()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector AlignmentVector = AlignmentResultPerBoid(i);

		NewCalculatedVelocityPerBoid[i] += AlignmentVector;
		// Boids[i]->Velocity += AlignmentVector;
	}	
}

FVector BoidsManager::AlignmentResultPerBoid(int BoidIndexToCalculate)
{
	FVector FinalAlignmentVector = FVector::ZeroVector;
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

void BoidsManager::UpdateCohesion()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector CohesionVector = CohesionResultPerBoid(i);
		
		NewCalculatedVelocityPerBoid[i] += CohesionVector;
		//Boids[i]->Velocity += CohesionVector;
	}	
}

FVector BoidsManager::CohesionResultPerBoid(int BoidIndexToCalculate)
{
	FVector FinalCohesionVector = FVector::ZeroVector;
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

void BoidsManager::CheckOutOfBounds()
{
	int HalfBound = BOIDS_BOUNDS / 2;
	
	for (auto CurrentBoid: Boids)
	{
		if (CurrentBoid->Position.X > HalfBound)
		{
			CurrentBoid->Position.X -= BOIDS_BOUNDS;
		}
		else if (CurrentBoid->Position.X < -HalfBound)
		{
			CurrentBoid->Position.X += BOIDS_BOUNDS;
		}

		if (CurrentBoid->Position.Y > HalfBound)
		{
			CurrentBoid->Position.Y -= BOIDS_BOUNDS;
		}
		else if (CurrentBoid->Position.Y < -HalfBound)
		{
			CurrentBoid->Position.Y += BOIDS_BOUNDS;
		}

		if (CurrentBoid->Position.Z > HalfBound)
		{
			CurrentBoid->Position.Z -= BOIDS_BOUNDS;
		}
		else if (CurrentBoid->Position.Z < -HalfBound)
		{
			CurrentBoid->Position.Z += BOIDS_BOUNDS;
		}
	}
}

