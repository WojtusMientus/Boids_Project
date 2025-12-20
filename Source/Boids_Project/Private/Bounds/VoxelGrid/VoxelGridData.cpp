
#include "Bounds/VoxelGrid/VoxelGridData/VoxelGridData.h"
#include "DataAssets/BoundsData.h"

FEnvironmentCollisionVoxelGridData::FEnvironmentCollisionVoxelGridData(const UBoundsData* BoundsData)
{
	OverwriteData(BoundsData);
}

void FEnvironmentCollisionVoxelGridData::OverwriteData(const UBoundsData* BoundsData)
{
	if (!BoundsData)
	{
		return;
	}
	
	VoxelGridData.BoundsData.Center = BoundsData->Center;
	VoxelGridData.BoundsData.Extent = BoundsData->Extent;
	VoxelGridData.BoundsData.GridResolution = BoundsData->GridResolution;
	
	VoxelGridData.VoxelGridData = BoundsData->GetCollisionForcesArray(); 
	
	EnvironmentCollisionMultiplier = BoundsData->EnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = BoundsData->BoundsCollisionMultiplier;
}
