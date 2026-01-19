// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentCollisionCellData.generated.h"


/**
 * Helper struct used by WorldCollisionVoxelGrid for boid to static geometry collision calculation.
 */
USTRUCT()
struct BOIDS_PROJECT_API FEnvironmentCollisionCellData
{
	
	GENERATED_BODY()
	
	UPROPERTY()
	/** Precalculated force from surrounding environment. */
	FVector EnvironmentCollisionForce = FVector::ZeroVector;
	
	UPROPERTY()
	/** Precalculated force from nearby simulation bounds. */
	FVector BoundsCollisionForce = FVector::ZeroVector;
	
	UPROPERTY()
	/** Flag determining if this voxel is simulation area or a wall. */
	bool bIsSimulationArea = false;
};