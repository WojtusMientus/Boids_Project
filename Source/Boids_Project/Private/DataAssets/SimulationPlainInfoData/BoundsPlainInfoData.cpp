
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"
#include "DataAssets/BoundsData.h"

FCollisionBoundsPlainInfo::FCollisionBoundsPlainInfo(const UBoundsData* NewBoundsData)
{
	OverwriteData(NewBoundsData);
}

void FCollisionBoundsPlainInfo::OverwriteData(const UBoundsData* NewBoundsData)
{
	if (!NewBoundsData)
	{
		return;
	}
	
	BoundsPlainData.Center = NewBoundsData->Center;
	BoundsPlainData.Extent = NewBoundsData->Extent;
	
	BoundsPlainData.EnvironmentGridResolution = NewBoundsData->EnvironmentGridResolution;
	BoundsPlainData.BoidCollisionGridResolution = NewBoundsData->BoidCollisionGridResolution;
	
	EnvironmentCollisionRows = NewBoundsData->EnvironmentCollisionRows;
	BoundsCollisionRows = NewBoundsData->BoundsCollisionRows;
	SimulationSeedVoxelIndices = NewBoundsData->SimulationSeedVoxelIndices;
}
