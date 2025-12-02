// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"


struct FEnvironmentCollisionData
{
	/** Precalculated force from surrounding environment. */
	FVector EnvironmentCollisionForce = FVector::ZeroVector;
	
	/** Precalculated force from nearby simulation bounds. */
	FVector BoundsCollisionForce = FVector::ZeroVector;
};

/**
 * Helper struct for passing saved data from saved assets at simulation startup.
 */
struct FVoxelGridCollisionData
{
	/** Struct storing essential bounds data and its content. */
	FVoxelGridData<FEnvironmentCollisionData>& VoxelGridData;
	
	/** Final multiplier applied to environment collision force before retrieving data. */
	float EnvironmentCollisionMultiplier = 1.0f;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	float BoundsCollisionMultiplier = 1.0f;
};


/**
 * Voxelized 3D bounding box storing 2 vectors in each cell representing forces from environment and simulation bounds.
 * Work in progress and currently unused - restructuring the class and moving responsibilities to CollisionDataGenerator.
 * Will only be data holder for runtime look up.
 */
class FWorldCollisionVoxelGrid: public FVoxelGrid<FEnvironmentCollisionData>
{
public:
	
	FWorldCollisionVoxelGrid(const FVoxelGridCollisionData& VoxelGridCollisionData);
	
	//~ Begin FVoxelGrid Interface
	virtual void InitializeVoxelGrid() override;
	//~ End FVoxelGrid Interface
	
	FVector GetFinalCollisionVectorAtIndex(const int32 Index) const;
	FVector GetFinalCollisionVectorAtLocation(const FVector& Location) const;
	
	void UpdateCollisionMultiplier(float InCollisionMultiplier);
	
private:
	
	/** Initializes the voxel grid and precomputes boundary forces. */
	void InitializeCollisionGrid();
	
	/** Generates collision rows near each wall of the bounds. Called only during object creation. */
	void InitializeForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	
	/** Applies additional force at given cell array index. */
	void AddForceAt(const FVector& Force, int32 Index);
	
	/** Final multiplier applied before retrieving collision force. */
	float CollisionMultiplier = 250.0f;
	
	/** Number of voxel collision rows that get generated near each bound and from static meshes. */
	static constexpr int32 COLLISION_ROWS = 5;
	
	/** Final multiplier applied to environment collision force before retrieving data. */
	float BoundsCollisionMultiplier;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	float EnvironmentCollisionMultiplier;
};

