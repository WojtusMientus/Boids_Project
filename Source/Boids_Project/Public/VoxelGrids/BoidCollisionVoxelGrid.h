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

	
	FORCEINLINE void GetNeighboringBoidSorted_TwoArrays(const FBoid& Boid, const FIntVector PerceptionDistance, const int32 SpeciesID,
									TArray<uint16>& OutSameNeighbors, TArray<FBoidCollisionCellData>& OutOtherNeighbors)
	{
		const FIntVector SpatialIndex = FBoundsMath::ClampToNearestBoundIndices(Boid.Position, 
			BoundsMin, VoxelCellSize, GridResolution);
		
		const FIntVector StartingIndices = FBoundsMath::ClampToBoundsResolution(SpatialIndex - PerceptionDistance,
		GridResolution);
		const FIntVector EndingIndices = FBoundsMath::ClampToBoundsResolution(SpatialIndex + PerceptionDistance,
			GridResolution);
		
		const int32 ResolutionMultiplierY = GridResolution.Z;
		const int32 ResolutionMultiplierX = GridResolution.Y * GridResolution.Z;
		
		for (int IndexX = StartingIndices.X; IndexX <= EndingIndices.X; IndexX++)
		{
			const int32 BaseX = IndexX * ResolutionMultiplierX;
		
			for (int IndexY = StartingIndices.Y; IndexY <= EndingIndices.Y; IndexY++)
			{
				const int32 BaseY = BaseX + IndexY * ResolutionMultiplierY;
			
				for (int IndexZ = StartingIndices.Z; IndexZ <= EndingIndices.Z; IndexZ++)
				{
					const int32 FinalArrayIndex = BaseY + IndexZ;

					for (const FBoidCollisionCellData& Cell : InternalVoxelGrid[FinalArrayIndex])
					{
						if (Cell.SpeciesIndex == SpeciesID)
						{
							OutSameNeighbors.Add(Cell.BoidIndex);
						}
						else
						{
							OutOtherNeighbors.Add(Cell);
						}
					}
				}
			}
		}		
	}
	
	FORCEINLINE void GetNeighboringBoidSorted_OneArray(const FBoid& Boid, const FIntVector PerceptionDistance,
	                                    TArray<TArray<uint16>>& OutNeighbors)
	{
		const FIntVector SpatialIndex = FBoundsMath::ClampToNearestBoundIndices(Boid.Position, 
			BoundsMin, VoxelCellSize, GridResolution);
		
		const FIntVector StartingIndices = FBoundsMath::ClampToBoundsResolution(SpatialIndex - PerceptionDistance,
		GridResolution);
		const FIntVector EndingIndices = FBoundsMath::ClampToBoundsResolution(SpatialIndex + PerceptionDistance,
			GridResolution);
		
		const int32 ResolutionMultiplierY = GridResolution.Z;
		const int32 ResolutionMultiplierX = GridResolution.Y * GridResolution.Z;
		
		for (int IndexX = StartingIndices.X; IndexX <= EndingIndices.X; IndexX++)
		{
			const int32 BaseX = IndexX * ResolutionMultiplierX;
		
			for (int IndexY = StartingIndices.Y; IndexY <= EndingIndices.Y; IndexY++)
			{
				const int32 BaseY = BaseX + IndexY * ResolutionMultiplierY;
			
				for (int IndexZ = StartingIndices.Z; IndexZ <= EndingIndices.Z; IndexZ++)
				{
					const int32 FinalArrayIndex = BaseY + IndexZ;
					
					for (const FBoidCollisionCellData& Cell : InternalVoxelGrid[FinalArrayIndex])
					{
						OutNeighbors[Cell.SpeciesIndex].Add(Cell.BoidIndex);
					}
				}
			}
		}		
	}
};
