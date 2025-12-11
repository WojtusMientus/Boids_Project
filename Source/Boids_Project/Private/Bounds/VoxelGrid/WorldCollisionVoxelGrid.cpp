#include "Bounds/VoxelGrid/WorldCollisionVoxelGrid.h"
#include "Bounds/VoxelGrid/VoxelGridData/EnvironmentCollisionCellData.h"


FWorldCollisionVoxelGrid::FWorldCollisionVoxelGrid(const FEnvironmentCollisionVoxelGridData& VoxelGridCollisionData): 
FVoxelGrid<FEnvironmentCollisionCellData>(VoxelGridCollisionData.VoxelGridData),
EnvironmentCollisionMultiplier(VoxelGridCollisionData.EnvironmentCollisionMultiplier),
BoundsCollisionMultiplier(VoxelGridCollisionData.BoundsCollisionMultiplier)
{
}

FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtIndex(const int32 Index) const
{
	const FEnvironmentCollisionCellData CollisionData = FVoxelGrid<FEnvironmentCollisionCellData>::GetValueAtIndex(Index);
	return CollisionData.EnvironmentCollisionForce * EnvironmentCollisionMultiplier + 
		CollisionData.BoundsCollisionForce * BoundsCollisionMultiplier;
}

FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtLocation(const FVector& Location) const
{
	const FEnvironmentCollisionCellData CollisionData = FVoxelGrid<FEnvironmentCollisionCellData>::GetValueAtLocation(Location);
	return CollisionData.EnvironmentCollisionForce * EnvironmentCollisionMultiplier + 
		CollisionData.BoundsCollisionForce * BoundsCollisionMultiplier;
}

void FWorldCollisionVoxelGrid::UpdateCollisionMultiplier(float InEnvironmentCollisionMultiplier, 
	float InBoundsCollisionMultiplier)
{
	EnvironmentCollisionMultiplier = InEnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = InBoundsCollisionMultiplier;
}
