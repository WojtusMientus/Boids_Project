// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"
#include "Utilities/Macros/DebugMacros.h"

#if WITH_EDITOR

#define ENSURE_ARRAY_INDEX_VALID(Index)\
	ENSURE_ALWAYS_MESSAGE_RETURN(InternalVoxelGrid.IsValidIndex(Index),\
	"No valid index.")

#define ENSURE_ARRAY_CELL_INDEX_VALID(GridIndex, CellIndex)\
	ENSURE_ALWAYS_MESSAGE_RETURN(InternalVoxelGrid[GridIndex].IsValidIndex(CellIndex),\
	"No valid cell index.")

#endif


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnBoidGridIndexChangedEvent, int32 BoidID, int32 GridIndex, 
	int32 GridCellIndex);


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
	
	/** Delegate broadcast whenever a Boid’s grid index is updated (after add, swap, or remove) */
	FOnBoidGridIndexChangedEvent OnBoidGridIndexChanged;
	
	/** Initializes every cell array with estimated number of Boids in cell. */
	void InitializeBoidCollisionGrid(const int32 StartingNumberOfBoids);
	
	void AddBoidToVoxelGrid(const int32 BoidID, const FVector& BoidPosition);
	void UpdateBoid(const FVector& BoidPosition, const int32 GridIndex, const int32 GridCellIndex);
	void RemoveBoid(const int32 GridIndex, const int32 GridCellIndex);
	
protected:
	
	/** Tries to swap Boid to the last place for faster deletion. */
	void TrySwapBoidToLastIndex(const int32 GridIndex, const int32 GridCellIndex);
	
	void AddBoidToVoxelGridInternal(const int32 BoidID, const int32 NewGridIndex);	
};
