// Copyright WojtusMientus

#pragma once

struct FCollisionData;
struct FCollisionBoundsPlainData;


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
	void SetupStartingValues(const FCollisionBoundsPlainData& CollisionBoundsData);
	FVector GetStartingCellCenter(const FVector& BoundsCenter, const FVector& Extent);
	float GetStartingCellCenterAxis(const float MinAxis, const float CellAxisSize);
	
	UWorld* GetCurrentEditorWorld();
	
	void ReserveEstimatedDataSize(const int32 NumberOfCells, FCollisionData& VisualizerData);
	float CalculateEstimatedDataSize(const int32 GridResolutionMin);
	
	void CalculateWallCollisionDataLocations(const UWorld* World, FCollisionData& VisualizerData);
	void CalculateCollisionForces(TArray<FVector>& OutCollisionForces);
	void AddCollisionToNeighborCells(const FIntVector& WallCell, TArray<FVector>& OutCollisionForces);
	
	
	FVector CalculateCollisionForceAt(const FIntVector& CurrentVoxel, const FIntVector& StartingVoxel);
	
	TSet<FIntVector> WallCollisionDataIndices;
	
	FIntVector GridResolution = FIntVector::ZeroValue;
	FVector VoxelCellSize = FVector::OneVector;
	FVector StartingCellCenter = FVector::Zero();
	FIntVector MaxCollisionBoundsDistanceVector = FIntVector::ZeroValue;
	
	
	int32 VoxelGridCellCount = 0;
	int32 EnvironmentCollisionRows = 1;
	int32 BoundsCollisionRows = 1;
	
	int32 MaxCollisionBoundsDistance = 2;
};
