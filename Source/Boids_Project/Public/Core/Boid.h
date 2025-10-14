// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"

/**
 * Simple Boid class representing position and velocity in 3D space.
 */
class BOIDS_PROJECT_API FBoid
{
	
public:

	/**
	 * Constructs a Boid with an initial position and velocity.
	 * @param InitPosition The starting world-space position.
	 * @param InitVelocity The starting velocity vector.
	 */
	FBoid(const FVector& InitPosition, const FVector& InitVelocity);	

	/**
	 * Updates the Boid's position based on its velocity.
	 * @param DeltaTime Time elapsed since the last frame.
	 */
	void Update(float DeltaTime);
	
	FVector Position;
	FVector Velocity;
	FVector Acceleration;
};
