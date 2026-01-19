
#include "VoxelGrids/BoidCollisionVoxelGrid.h"


void FBoidCollisionVoxelGrid::InitializeBoidDataCollisionGrid(
	const FVoxelGridData<TArray<FBoidCollisionCellData>>& VoxelGridData, const int32 StartingNumberOfBoids)
{
	InitializeVoxelGrid(VoxelGridData);
	
	for (TArray<FBoidCollisionCellData>& VoxelCellArray: InternalVoxelGrid)
	{
		VoxelCellArray.Reserve(StartingNumberOfBoids / 20);
	}
}
