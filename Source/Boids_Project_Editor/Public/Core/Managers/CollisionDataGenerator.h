// Copyright WojtusMientus

#pragma once

struct FCollisionBoundsPlainData;


/**
 * Manager responsible for generating voxelized collision data in certain area 
 * based in its center, extent and resolution.
 * Currently, it generates only wall data, without any vector repelling force data.
 */
class FCollisionDataGenerator
{
	
public:
	
	TArray<bool> GenerateCollisionData(const FCollisionBoundsPlainData& CollisionBoundsPlainData);
	
private:
	
	/** Helper functions. */
	void SetupStartingValues(const FVector& Center, const FVector& Extent, const FIntVector Resolution);
	FVector GetStartingCellCenter(const FVector& BoundsCenter, const FVector& Extent);
	float GetStartingCellCenterAxis(const float MinAxis, const float CellAxisSize);
	
	UWorld* GetCurrentEditorWorld();
	
	void CalculateCollisionData(const UWorld* World, TArray<bool>& OutCollisionData);
	

	FIntVector GridResolution = FIntVector::ZeroValue;
	FVector VoxelCellSize = FVector::OneVector;
	FVector StartingCellCenter = FVector::Zero();
	
};
