
#include "Bounds/VoxelGrid/WorldCollisionVoxelGrid.h"


void FWorldCollisionVoxelGrid::InitializeWorldCollisionVoxelGrid(
	const FEnvironmentCollisionVoxelGridData& EnvironmentCollisionVoxelGridData)
{
	InitializeVoxelGrid(EnvironmentCollisionVoxelGridData.VoxelGridData);
	
	EnvironmentCollisionMultiplier = EnvironmentCollisionVoxelGridData.EnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = EnvironmentCollisionVoxelGridData.BoundsCollisionMultiplier;
}

void FWorldCollisionVoxelGrid::UpdateCollisionMultiplier(float InEnvironmentCollisionMultiplier, 
	float InBoundsCollisionMultiplier)
{
	EnvironmentCollisionMultiplier = InEnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = InBoundsCollisionMultiplier;
}
