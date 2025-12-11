// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"


class UBoidManagerSubsystem;


DECLARE_DELEGATE_ThreeParams(FOnBoidGridIndexChangedSignature, int32 /* BoidID */, int32 /* GridIndex */, 
	int32 /* GridCellIndex */);


/**
 * Voxelized 3D bounding box storing Boid IDs for localized collision detection. 
 * Work in progress and currently unused until I benchmark it against the multithreaded O(n^2) solution.
 * Since it stores data in the array of arrays, it isn't safe for concurrent writing.
 * After doing all off the neighbor collision algorithm - updating the grid before next frame happens on single core.
 */
class FBoidCollisionVoxelGrid: public FVoxelGrid<TArray<int32>>
{
public:
	
	FBoidCollisionVoxelGrid(const FVoxelGridData<TArray<int32>>& VoxelGridData);
	
	/**  */
	FOnBoidGridIndexChangedSignature OnBoidGridIndexChanged;
	
	
	/**  */
	void InitializeBoidCollisionGrid(const int32 StartingNumberOfBoids);

	/**  */
	void AddBoidToVoxelGrid(const int32 BoidID, const FVector& BoidPosition);
	
	/**  */
	void UpdateBoid(const FVector& BoidPosition, const int32 GridIndex, const int32 GridCellIndex);
	
	/**  */
	void RemoveBoid(const int32 GridIndex, const int32 GridCellIndex);
	
protected:
	
	/**  */
	void TrySwapBoidToLastIndex(const int32 GridIndex, const int32 GridCellIndex);
	
	/**  */
	void AddBoidToVoxelGrid(const int32 BoidID, const int32 NewGridIndex);	
};
