// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"
#include "VoxelGrids/VoxelGridData/EnvironmentCollisionCellData.h"


/**
 * Voxelized 3D bounding box storing 2 vectors in each cell representing forces from environment and simulation bounds.
 * Works as a data holder for runtime look up.
 */
class FWorldCollisionVoxelGrid: public FVoxelGrid<FEnvironmentCollisionCellData>
{
	
public:
	
	virtual void InitializeWorldCollisionVoxelGrid(const 
		FEnvironmentCollisionVoxelGridData& EnvironmentCollisionVoxelGridData);
	
	FORCEINLINE FVector GetFinalCollisionVectorAtIndex(const int32 Index, 
		const float EnvironmentCollisionMultiplier, const float BoundsCollisionMultiplier) const;
	FORCEINLINE FVector GetFinalCollisionVectorAtLocation(const FVector& Location, 
		const float EnvironmentCollisionMultiplier, const float BoundsCollisionMultiplier) const;
	
	virtual FVector GetRandomPointInsideGrid() const override;
	
};

inline FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtIndex(const int32 Index, 
	const float EnvironmentCollisionMultiplier, const float BoundsCollisionMultiplier) const
{
	const FEnvironmentCollisionCellData CollisionData = GetValueAtIndex(Index);
	return CollisionData.EnvironmentCollisionForce * EnvironmentCollisionMultiplier + 
		CollisionData.BoundsCollisionForce * BoundsCollisionMultiplier;
}

inline FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtLocation(const FVector& Location, 
	const float EnvironmentCollisionMultiplier, const float BoundsCollisionMultiplier) const
{
	return GetFinalCollisionVectorAtIndex(PositionToArrayIndex(Location), 
		EnvironmentCollisionMultiplier, BoundsCollisionMultiplier);
}