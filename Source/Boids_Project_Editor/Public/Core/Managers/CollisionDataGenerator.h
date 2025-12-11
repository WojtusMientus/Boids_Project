// Copyright WojtusMientus

#pragma once
#include "Core/CollisionData.h"

struct FCollisionData;
struct FCollisionBoundsPlainInfo;


/**
 * Manager responsible for generating voxelized collision data in certain area 
 * based in its center, extent and resolution.
 * Currently, it generates only wall data, without any vector repelling force data.
 */
class FCollisionDataGenerator
{
	
public:
	
	void GenerateCollisionData(FCollisionData& CollisionDat);
	
private:
	
	/** Helper functions. */
	void SetupStartingValues(const FCollisionBoundsPlainInfo& CollisionBoundsData);
	
	UWorld* GetCurrentEditorWorld();
	
	void ReserveEstimatedDataSize(const int32 NumberOfCells, FCollisionData& VisualizerData);
	float CalculateEstimatedDataSize(const int32 GridResolutionMin);
	
	void CalculateWallCollisionDataLocations(const UWorld* World, FCollisionData& VisualizerData);
	void CalculateCollisionForces(TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	
	void AddWallCollisionForceToNeighborCells(const FIntVector& WallCell, TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	FVector CalculateWallCollisionForceAt(const FIntVector& CurrentVoxel, const FIntVector& StartingVoxel);
	
	void CalculateBoundsCollisionForces(TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	void CalculateForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary, 
		TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	void CalculateForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary, 
	TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	void CalculateForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary, 
	TArray<FEnvironmentCollisionCellData>& OutCollisionForces);
	
	TSet<FIntVector> WallCollisionDataIndices;
	
	FIntVector GridResolution = FIntVector::ZeroValue;
	FVector VoxelCellSize = FVector::OneVector;
	FVector StartingCellCenter = FVector::Zero();
	FIntVector MaxCollisionBoundsDistanceVector = FIntVector::ZeroValue;
	
	int32 VoxelGridCellCount = 0;
	int32 BoundsCollisionRows = 1;
	
	int32 MaxCollisionBoundsDistance = 2;
};
