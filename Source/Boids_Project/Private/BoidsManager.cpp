// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidsManager.h"
#include "Boid.h"

BoidsManager::BoidsManager()
{
	InitializeBoids();
}

BoidsManager::~BoidsManager()
{
}

void BoidsManager::InitializeBoids()
{
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		Boids.Add(MakeShared<Boid>(BOIDS_BOUNDS));
	}
}

void BoidsManager::UpdateBoids()
{
	UpdateAlignment();
	UpdateCohesion();
	UpdateSeparation();
	
	for (int i = 0; i < BOIDS_COUNT; i++)
	{
		if (Boids[i]->Velocity.Size() > BOID_MAX_SPEED)
		{
			Boids[i]->Velocity.Normalize();
			Boids[i]->Velocity *= BOID_MAX_SPEED;
		}
	}
	
	for (auto CurrentBoid: Boids)
	{
		CurrentBoid->Update();
	}
	
	CheckOutOfBounds();
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

void BoidsManager::UpdateSeparation()
{
	for (int i = 0; i < Boids.Num(); i++)
	{
		FVector SeparationVector = SeparationResultPerBoid(i);

		Boids[i]->Velocity += SeparationVector;
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
			DesiredDirection *= (1 - FMath::Pow(DistanceToOtherBoid / PERCEPTION_DISTANCE, 2));
			
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
		FVector AllignmetVector = AlignmentResultPerBoid(i);

		Boids[i]->Velocity += AllignmetVector;
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
			FinalAlignmentVector += Boids[i]->Velocity;				
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

		Boids[i]->Velocity += CohesionVector;
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

