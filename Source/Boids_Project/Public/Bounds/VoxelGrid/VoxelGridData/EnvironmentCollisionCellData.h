
#pragma once

#include "CoreMinimal.h"
#include "EnvironmentCollisionCellData.generated.h"

USTRUCT()
struct BOIDS_PROJECT_API FEnvironmentCollisionCellData
{
	
	GENERATED_BODY()
	
	/** Precalculated force from surrounding environment. */
	FVector EnvironmentCollisionForce = FVector::ZeroVector;
	
	/** Precalculated force from nearby simulation bounds. */
	FVector BoundsCollisionForce = FVector::ZeroVector;
};