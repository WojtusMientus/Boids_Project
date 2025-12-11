
#include "DataAssets/BoundsData.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"

void UBoundsData::OverwriteData(const FCollisionBoundsPlainInfo& NewCollisionBoundsData,
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
