// Copyright WojtusMientus

// Simple Boid class representing position and velocity
// Provides basic Update() functionality for demo purposes

#pragma once

#include "CoreMinimal.h"

class BOIDS_PROJECT_API Boid
{
	
public:

	// ----- Constructors -----
	Boid();
	Boid(float MaxBounds);
	~Boid() = default;

	// ----- Public API -----
	void Update(float DeltaTime, float MaxSpeed);
	
	FVector Position;
	FVector Velocity;
};
