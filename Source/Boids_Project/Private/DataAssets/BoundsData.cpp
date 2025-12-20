
#include "DataAssets/BoundsData.h"

#include "Bounds/VoxelGrid/VoxelGridData/VoxelGridData.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"

TArray<FEnvironmentCollisionCellData> UBoundsData::GetCollisionForcesArray() const
{
	return CollisionForces;
}

void UBoundsData::OverwritePlainData(const FCollisionBoundsPlainInfo& NewCollisionBoundsData,
                                     const TArray<FEnvironmentCollisionCellData>& NewCollisionWallData)
{
	Center = NewCollisionBoundsData.BoundsPlainData.Center;
	Extent = NewCollisionBoundsData.BoundsPlainData.Extent;
	GridResolution = NewCollisionBoundsData.BoundsPlainData.GridResolution;
	EnvironmentCollisionRows = NewCollisionBoundsData.EnvironmentCollisionRows;
	BoundsCollisionMultiplier = NewCollisionBoundsData.BoundsCollisionMultiplier;
	CollisionForces = NewCollisionWallData;
	CollisionArraySize = GridResolution;
}

FCollisionBoundsPlainInfo UBoundsData::GetPlainDataInfo() const
{
	return FCollisionBoundsPlainInfo(this);
}

FEnvironmentCollisionVoxelGridData UBoundsData::GetVoxelGridData() const
{
	return FEnvironmentCollisionVoxelGridData(this);
}
