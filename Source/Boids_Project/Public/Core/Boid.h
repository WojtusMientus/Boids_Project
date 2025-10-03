// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"

/**
 * Simple Boid class representing position and velocity in 3D space.
 */
class BOIDS_PROJECT_API FBoid
{
	
public:

	/** Default constructor. */
	FBoid();

	/**
	 * Constructs a Boid with an initial position and velocity.
	 * @param Position The starting world-space position.
	 * @param Velocity The starting velocity vector.
	 */
	FBoid(const FVector& Position, const FVector& Velocity);	
	~FBoid() = default;

	/**
	 * Updates the Boid's position based on its velocity.
	 * @param DeltaTime Time elapsed since the last frame.
	 */
	void Update(float DeltaTime);

	/** The Boid's current location in 3D space. */
	FVector Position;

	/** The Boid's current movement speed. */
	FVector Velocity;

	/** The Boid's current acceleration. */
	FVector Acceleration;
};
