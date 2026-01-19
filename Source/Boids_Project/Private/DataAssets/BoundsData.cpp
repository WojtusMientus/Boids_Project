
#include "DataAssets/BoundsData.h"
#include "VoxelGrids/VoxelGridData/VoxelGridData.h"
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
	
	EnvironmentGridResolution = NewCollisionBoundsData.BoundsPlainData.EnvironmentGridResolution;
	BoidCollisionGridResolution = NewCollisionBoundsData.BoundsPlainData.BoidCollisionGridResolution;
	
	EnvironmentCollisionRows = NewCollisionBoundsData.EnvironmentCollisionRows;
	BoundsCollisionRows = NewCollisionBoundsData.BoundsCollisionRows;
	SimulationSeedVoxelIndices = NewCollisionBoundsData.SimulationSeedVoxelIndices;
	
	CollisionForces = NewCollisionWallData;
	CollisionArraySize = EnvironmentGridResolution;
}

