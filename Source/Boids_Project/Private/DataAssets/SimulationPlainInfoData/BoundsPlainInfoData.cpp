
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
	BoundsPlainData.GridResolution = NewBoundsData->GridResolution;
	EnvironmentCollisionRows = NewBoundsData->EnvironmentCollisionRows;
	BoundsCollisionRows = NewBoundsData->BoundsCollisionRows;
	EnvironmentCollisionMultiplier = NewBoundsData->EnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = NewBoundsData->BoundsCollisionMultiplier;
}
