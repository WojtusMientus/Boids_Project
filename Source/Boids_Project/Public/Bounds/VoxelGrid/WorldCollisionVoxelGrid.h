// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"
#include "Bounds/VoxelGrid/VoxelGridData/EnvironmentCollisionCellData.h"



/**
 * Voxelized 3D bounding box storing 2 vectors in each cell representing forces from environment and simulation bounds.
 * Work in progress and currently unused - restructuring the class and moving responsibilities to CollisionDataGenerator.
 * Will only be data holder for runtime look up.
 */
class FWorldCollisionVoxelGrid: public FVoxelGrid<FEnvironmentCollisionCellData>
{
	
public:
	
	virtual void InitializeWorldCollisionVoxelGrid(const 
		FEnvironmentCollisionVoxelGridData& EnvironmentCollisionVoxelGridData);
	
	FORCEINLINE FVector GetFinalCollisionVectorAtIndex(const int32 Index) const;
	FORCEINLINE FVector GetFinalCollisionVectorAtLocation(const FVector& Location) const;
	
	void UpdateCollisionMultiplier(float InEnvironmentCollisionMultiplier, float InBoundsCollisionMultiplier);

private:
	
	/** Final multiplier applied to environment collision force before retrieving data. */
	float EnvironmentCollisionMultiplier = 0.0f;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	float BoundsCollisionMultiplier = 0.0f;
};

inline FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtIndex(const int32 Index) const
{
	const FEnvironmentCollisionCellData CollisionData = GetValueAtIndex(Index);
	return CollisionData.EnvironmentCollisionForce * EnvironmentCollisionMultiplier + 
		CollisionData.BoundsCollisionForce * BoundsCollisionMultiplier;
}

inline FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtLocation(const FVector& Location) const
{
	return GetFinalCollisionVectorAtIndex(PositionToArrayIndex(Location));
}