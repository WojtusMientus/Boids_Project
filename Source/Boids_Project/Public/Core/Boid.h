// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"


/**
 * Simple Boid class representing position, velocity and acceleration in 3D space.
 */
class BOIDS_PROJECT_API FBoid
{
	
public:
	
	FBoid(const FVector& InitPosition = FVector::ZeroVector, const FVector& InitVelocity = FVector::ZeroVector);	

	void Initialize(const FVector& InitPosition = FVector::ZeroVector, 
	const FVector& InitVelocity = FVector::ZeroVector);
	void Reset();
	
	void Update(float DeltaTime);
	
	
	FVector Position = FVector::ZeroVector;
	FVector Velocity = FVector::ZeroVector;
	FVector Acceleration = FVector::ZeroVector;
};
