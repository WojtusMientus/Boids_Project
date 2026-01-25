
#include "VoxelGrids/WorldCollisionVoxelGrid.h"


void FWorldCollisionVoxelGrid::InitializeWorldCollisionVoxelGrid(
	const FEnvironmentCollisionVoxelGridData& EnvironmentCollisionVoxelGridData)
{
	InitializeVoxelGrid(EnvironmentCollisionVoxelGridData.EnvironmentGridData);
}

FVector FWorldCollisionVoxelGrid::GetRandomPointInsideGrid() const
{
	int32 RandomLinearGridIndex;
	FEnvironmentCollisionCellData EnvironmentCollisionVoxelGridData;

	do
	{
		RandomLinearGridIndex = FMath::RandRange(0, InternalVoxelGrid.Num() - 1);
		EnvironmentCollisionVoxelGridData = InternalVoxelGrid[RandomLinearGridIndex];
	} while (!EnvironmentCollisionVoxelGridData.bIsSimulationArea);
	
	return FBoundsMath::GetRandomPositionInVoxelCell(RandomLinearGridIndex, GetGridCenter(), GetExtent(), GridResolution);
}
