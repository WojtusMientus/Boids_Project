// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "BoidCollisionCellData.h"
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
 * Helper struct for passing saved environment collision data from assets at simulation startup.
 */
struct FEnvironmentCollisionVoxelGridData
{
	/** Struct storing essential bounds data and its content. */
	FVoxelGridData<FEnvironmentCollisionCellData> EnvironmentGridData;
	
	
	FEnvironmentCollisionVoxelGridData() {};
	FEnvironmentCollisionVoxelGridData(const UBoundsData* BoundsData);
	
	void OverwriteData(const UBoundsData* BoundsData);
};

/**
 * Helper struct for passing saved boid collision data from assets at simulation startup.
 */
struct FBoidCollisionVoxelGridData
{
	/** Struct storing essential bounds data and its content. */
	FVoxelGridData<TArray<FBoidCollisionCellData>> BoidGridData;
	
	
	FBoidCollisionVoxelGridData() {};
	FBoidCollisionVoxelGridData(const UBoundsData* BoundsData);
	
	void OverwriteData(const UBoundsData* BoundsData);
	void InitializeBoidCollisionArray();
};
