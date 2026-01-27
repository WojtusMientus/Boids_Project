// Copyright WojtusMientus

#pragma once

#include "VoxelGrid.h"
#include "Core/Boid.h"
#include "VoxelGridData/BoidCollisionCellData.h"

struct FBoidCollisionCellData;


/**
 * Voxelized 3D bounding box storing Boid IDs for localized collision detection.
 * Needs to be rebuilt every frame for proper collision detection. 
 * Benchmarked as rebuilding was faster than updating.
 */
class FBoidCollisionVoxelGrid: public FVoxelGrid<TArray<FBoidCollisionCellData>>
{
public:
	
	/** Initializes every cell array with estimated number of Boids in cell. */
	void InitializeBoidDataCollisionGrid(const FVoxelGridData<TArray<FBoidCollisionCellData>>& VoxelGridData, 
		const int32 StartingNumberOfBoids);
	
	FORCEINLINE void ResetVoxelGrid()
	{
		for (TArray<FBoidCollisionCellData>& VoxelCellArray: InternalVoxelGrid)
		{
			VoxelCellArray.Reset();
		}
	}
	
	FORCEINLINE void AddBoidToVoxelGrid(FBoid& Boid, const int32 SpeciesID, const int32 BoidID)
	{
		const int32 GridIndex = PositionToArrayIndex(Boid.Position);
		InternalVoxelGrid[GridIndex].Add(FBoidCollisionCellData(SpeciesID, BoidID));
	}
	
	FORCEINLINE void GetNeighboringBoidInVoxelGrid(const FBoid& Boid, const FIntVector PerceptionDistance,
	                                    TArray<TArray<uint16>>& OutNeighbors)
	{
		const FIntVector SpatialIndex = FBoundsMath::ClampToNearestBoundIndices(Boid.Position, 
			BoundsMin, VoxelCellSize, GridResolution);
		
		const FIntVector StartingIndices = FBoundsMath::ClampToBoundsResolution(SpatialIndex - PerceptionDistance,
		GridResolution);
		const FIntVector EndingIndices = FBoundsMath::ClampToBoundsResolution(SpatialIndex + PerceptionDistance,
			GridResolution);
		
		for (int IndexX = StartingIndices.X; IndexX <= EndingIndices.X; IndexX++)
		{
			for (int IndexY = StartingIndices.Y; IndexY <= EndingIndices.Y; IndexY++)
			{
				for (int IndexZ = StartingIndices.Z; IndexZ <= EndingIndices.Z; IndexZ++)
				{
					const int32 FinalArrayIndex = FBoundsMath::XYZToArrayIndex(IndexX, IndexY, IndexZ, GridResolution);
					
					for (const FBoidCollisionCellData& Cell : InternalVoxelGrid[FinalArrayIndex])
					{
						OutNeighbors[Cell.SpeciesIndex].Add(Cell.BoidIndex);
					}
				}
			}
		}		
	}
};
