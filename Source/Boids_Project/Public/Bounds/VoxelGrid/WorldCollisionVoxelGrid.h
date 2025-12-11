// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"


/**
 * Voxelized 3D bounding box storing 2 vectors in each cell representing forces from environment and simulation bounds.
 * Work in progress and currently unused - restructuring the class and moving responsibilities to CollisionDataGenerator.
 * Will only be data holder for runtime look up.
 */
class FWorldCollisionVoxelGrid: public FVoxelGrid<FEnvironmentCollisionCellData>
{
public:
	
	FWorldCollisionVoxelGrid(const FEnvironmentCollisionVoxelGridData& VoxelGridCollisionData);
	
	FVector GetFinalCollisionVectorAtIndex(const int32 Index) const;
	FVector GetFinalCollisionVectorAtLocation(const FVector& Location) const;
	
	void UpdateCollisionMultiplier(float InEnvironmentCollisionMultiplier, float InBoundsCollisionMultiplier);
	
private:
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	float EnvironmentCollisionMultiplier;
	
	/** Final multiplier applied to environment collision force before retrieving data. */
	float BoundsCollisionMultiplier;
};

