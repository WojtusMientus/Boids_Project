// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Bounds/Bounds.h"

/**
 * Represents a voxelized collision boundary.
 * Stores precomputed forces in each voxel to push Boids away, creating a "soft wall" effect.
 */
class BOIDS_PROJECT_API FWorldCollisionBounds: public FBounds
{
	
public:

	/** Default constructor. Initializes voxel grid. */
	FWorldCollisionBounds();
	/** Constructs a bounds from the given min and max corners. Initializes voxel grid. */
	FWorldCollisionBounds(const FVector& MaxCorner, const FVector& MinCorner);
	/** Constructs a bounding box of the given size, centered ath the origin (0,0,0). Initializes voxel grid. */
	FWorldCollisionBounds(const float BoundsSize);
	~FWorldCollisionBounds() = default;

	/**
	 * Updates bounds world position and size.
	 * @param NewCenter World location of the Bounds.
	 * @param NewBoxExtent Extent of the Bounds.
	 */
	void UpdateBounds(const FVector& NewCenter, const FVector& NewBoxExtent);

	/**
	 * Returns collision force at given location.
	 * @param Location Position to sample on.
	 */
	FVector GetCollisionForceAt(const FVector& Location) const;

	/**
	 * Returns voxel center at given indices.
	 * @param IndexX The grid index along the X-axis.
	 * @param IndexY The grid index along the Y-axis.
	 * @param IndexZ The grid index along the Z-axis.
	 */
	FVector GetCellCenter(int32 IndexX, int32 IndexY, int32 IndexZ) const;

private:

	/** Grid resolution along the X axis. */
	static constexpr int32 DIMENSION_X = 35;
	
	/** Grid resolution along the Y axis. */
	static constexpr int32 DIMENSION_Y = 35;
	
	/** Grid resolution along the Z axis. */
	static constexpr int32 DIMENSION_Z = 35;
	
	/** Precomputed multiplier for XY plane indexing. */
	static constexpr int32 DIMENSION_XY = DIMENSION_X * DIMENSION_Y;

	/** Number of voxel collision rows that get generated near each bound. Used only during object creation. */
	static constexpr int32 COLLISION_ROWS = 4;

	/** Final multiplier applied before retrieving collision force. */
	static constexpr float COLLISION_MULTIPLIER = .15f;

	/** Calculated voxel sizes along each axis. Recalculated every bounds change. */
	float VoxelSizeX = 0.0f;
	float VoxelSizeY = 0.0f;
	float VoxelSizeZ = 0.0f;
	
	/** Voxel grid storing precomputed collision forces. */
	TArray<FVector> VoxelCollisionGrid;	

	/** Initializes the voxel grid and precomputes boundary forces. */
	void InitializeCollisionGrid();
	
	/** Generates collision rows near each wall of the bounds. Called only during object creation. */
	void InitializeForcesNearBounds();
	void InitializeForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);
	void InitializeForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector, bool bAtLowerBoundary);

	/** Calculating the voxel size for each dimension. */
	void CalculateCellSizes();
	
	/** Applies additional force at given cell array index. */
	void AddForceAt(const FVector& Force, int32 Index);

	/**
	 * Converts 3D grid indices to a 1D array index.
	 * @param X The grid index along the X-axis.
	 * @param Y The grid index along the Y-axis.
	 * @param Z The grid index along the Z-axis.
	 */
	FORCEINLINE int32 ToArrayIndex(int32 X, int32 Y, int32 Z) const
	{
		return X + Y * DIMENSION_X + Z * DIMENSION_XY;
	}

	/**
	 * Computes the voxel X-axis index for a given world location.
	 * @param Location World location to convert.
	 */
	FORCEINLINE int32 CellIndexX(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.X - BoundsMin.X) / VoxelSizeX), 0, DIMENSION_X - 1);
	}

	/**
	 * Computes the voxel Y-axis index for a given world location.
	 * @param Location World location to convert.
	 */
	FORCEINLINE int32 CellIndexY(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Y - BoundsMin.Y) / VoxelSizeY), 0, DIMENSION_Y - 1);
	}

	/**
	 * Computes the voxel Z-axis index for a given world location.
	 * @param Location World location to convert.
	 */
	FORCEINLINE int32 CellIndexZ(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Z - BoundsMin.Z) / VoxelSizeZ), 0, DIMENSION_Z - 1);
	}
};
