// Copyright WojtusMientus

#pragma once

#include "Core/CollisionData.h"

struct FCollisionData;
struct FCollisionBoundsPlainInfo;


/**
 * Manager responsible for generating voxelized collision data in simulation area.
 */
class FCollisionDataGenerator
{
	
public:
	
	void GenerateCollisionData_NEW(FCollisionData& CollisionData);
	
private:
	
	/** Helper functions. */
	void SetupStartingValues(const FCollisionBoundsPlainInfo& CollisionBoundsData);
	
	UWorld* GetCurrentEditorWorld() const;
		
	void CalculateWallCollisionData(const UWorld* World, FCollisionData& VisualizerData, const FIntVector& StartingIndex);
	
	void InitializeHelperDataStructures(const FIntVector& StartingIndex);
	void TryAddNeighboringVoxels(const FIntVector& CurrentVoxel);
	void TryAddNeighborVoxel(const FIntVector& CurrentVoxel, const FIntVector& Direction);
	
	void CalculateCollisionForcesAroundWalls(TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	
	void AddWallCollisionForceToNeighborCells(const FIntVector& WallCell, TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	FVector CalculateWallCollisionForceAt(const FIntVector& CurrentVoxel, const FIntVector& StartingVoxel);
	
	void CalculateBoundsCollisionForces(TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	void CalculateForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary, 
		TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	void CalculateForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary, 
		TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	void CalculateForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary, 
		TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	
	
	/** Set storting most outer wall collision data for later collision forces generation. */
	TSet<FIntVector> EnvironmentOuterWalls;
	
	/** Set storting indices of already visited voxels in Flood Fill algorithm. */
	TSet<FIntVector> AlreadyAddedVoxels;
	
	/** Array used as a stack for next in line voxels in Flood Fill algorithm. */
	TArray<FIntVector> VoxelsToCompute;
	

	FIntVector GridResolution = FIntVector::ZeroValue;
	FVector VoxelCellSize = FVector::OneVector;
	FVector StartingCellCenter = FVector::Zero();
	
	int32 VoxelGridCellCount = 0;
	
	int32 BoundsCollisionRows = 1;
	FIntVector EnvironmentCollisionRows = FIntVector::ZeroValue;
};
