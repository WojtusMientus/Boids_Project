
#pragma once

#include "CoreMinimal.h"
#include "EnvironmentCollisionCellData.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"


template<class T>
struct FVoxelGridData
{
	/** Struct holding information about center, extent and the resolution of the grid. */
	FBoundsPlainInfo BoundsData = FBoundsPlainInfo();
	
	/** Array representing voxel grid storing T data. */
	TArray<T> VoxelGridData = TArray<T>();
};

/**
 * Helper struct for passing saved data from saved assets at simulation startup.
 */
struct FEnvironmentCollisionVoxelGridData
{
	/** Struct storing essential bounds data and its content. */
	FVoxelGridData<FEnvironmentCollisionCellData> VoxelGridData;
	
	/** Final multiplier applied to environment collision force before retrieving data. */
	float EnvironmentCollisionMultiplier = 1.0f;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	float BoundsCollisionMultiplier = 1.0f;
	
	FEnvironmentCollisionVoxelGridData() {};
	FEnvironmentCollisionVoxelGridData(const UBoundsData* BoundsData);
	
	void OverwriteData(const UBoundsData* BoundsData);
};
