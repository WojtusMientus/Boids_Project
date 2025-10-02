// Copyright WojtusMientus

// Represents a voxelized collision boundary around the demo world.
// Each voxel stores precomputed collision forces near the bounds.
// This design can be extended later to support richer collision data.
//
//  Future: Store per-voxel data of static geometry collision

#pragma once

#include "CoreMinimal.h"
#include "Bounds/Bounds.h"


class BOIDS_PROJECT_API FWorldCollisionBounds: public FBounds
{
	
public:

	// ----- Constructors -----
	FWorldCollisionBounds();
	FWorldCollisionBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	FWorldCollisionBounds(const float BoundsSize);
	~FWorldCollisionBounds() = default;

	// ----- Public API ----- 
	void UpdateBounds(const FVector& Center, const FVector& BoxExtent);
	
	FVector GetCollisionForceAt(const FVector& Location) const;
	FVector GetCellCenter(int32 IndexX, int32 IndexY, int32 IndexZ) const;
	
private:

	// ----- Grid Configuration -----
	static constexpr int32 DIMENSION_X = 35;
	static constexpr int32 DIMENSION_Y = 35;
	static constexpr int32 DIMENSION_Z = 35;
	static constexpr int32 DIMENSION_XY = DIMENSION_X * DIMENSION_Y;
	
	static constexpr int32 COLLISION_ROWS = 4;
	static constexpr float COLLISION_MULTIPLIER = .15f;

	// ----- Runtime Variables -----
	float CellSizeX = 0.0f;
	float CellSizeY = 0.0f;
	float CellSizeZ = 0.0f;

	// Stores precomputed per-voxel collision forces
	TArray<FVector> VoxelCollisionGrid;	
	
	
	// ----- Initialization -----
	void InitializeForcesNearBounds();
	void InitializeForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeCellSizes();
	
	// ----- Private Helpers -----
	void AddForceAt(const FVector& Force, int32 Index);
	
	FORCEINLINE int32 ToArrayIndex(int32 X, int32 Y, int32 Z) const
	{
		return X + Y * DIMENSION_X + Z * DIMENSION_XY;
	}

	FORCEINLINE int32 CellIndexX(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.X - BoundsBottomLeftBackCorner.X) / CellSizeX), 0, DIMENSION_X - 1);
	}

	FORCEINLINE int32 CellIndexY(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Y - BoundsBottomLeftBackCorner.Y) / CellSizeY), 0, DIMENSION_Y - 1);
	}

	FORCEINLINE int32 CellIndexZ(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Z - BoundsBottomLeftBackCorner.Z) / CellSizeZ), 0, DIMENSION_Z - 1);
	}
};
