
#pragma once

#include "CoreMinimal.h"

struct BOIDS_PROJECT_API FEnvironmentCollisionCellData
{
	/** Precalculated force from surrounding environment. */
	FVector EnvironmentCollisionForce = FVector::ZeroVector;
	
	/** Precalculated force from nearby simulation bounds. */
	FVector BoundsCollisionForce = FVector::ZeroVector;
};