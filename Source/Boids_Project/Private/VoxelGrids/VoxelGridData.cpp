
#include "VoxelGrids/VoxelGridData/VoxelGridData.h"
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
	
	EnvironmentGridData.BoundsData.Center = BoundsData->Center;
	EnvironmentGridData.BoundsData.Extent = BoundsData->Extent;
	EnvironmentGridData.BoundsData.GridResolution = BoundsData->EnvironmentGridResolution;
	
	EnvironmentGridData.VoxelGridData = BoundsData->GetCollisionForcesArray(); 
}


FBoidCollisionVoxelGridData::FBoidCollisionVoxelGridData(const UBoundsData* BoundsData)
{
	OverwriteData(BoundsData);
}

void FBoidCollisionVoxelGridData::OverwriteData(const UBoundsData* BoundsData)
{
	if (!BoundsData)
	{
		return;
	}
	
	BoidGridData.BoundsData.Center = BoundsData->Center;
	BoidGridData.BoundsData.Extent = BoundsData->Extent;
	BoidGridData.BoundsData.GridResolution = BoundsData->BoidCollisionGridResolution;
}

void FBoidCollisionVoxelGridData::InitializeBoidCollisionArray()
{
	const FIntVector GridResolution =  BoidGridData.BoundsData.GridResolution;
	const int32 GridSize = GridResolution.X * GridResolution.Y * GridResolution.Z;
	BoidGridData.VoxelGridData.SetNumZeroed(GridSize);
}

