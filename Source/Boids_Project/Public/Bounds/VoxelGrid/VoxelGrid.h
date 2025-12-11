// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "VoxelGridData/VoxelGridData.h"

/**
 * Template class representing voxelized 3D axis-aligned bounding box in the array. (AABB)
 * @tparam T Data type stored in each voxel cell. 
 */
template<class T>
class FVoxelGrid
{
public:
	
	FVoxelGrid(const FVoxelGridData<T>& VoxelGridData);
	virtual ~FVoxelGrid() = default;
	
	/** Returns the size of the voxel grid (Max - Min). */
	FORCEINLINE FVector Get3DSize() const { return BoundsMax - BoundsMin; }

	/** Returns the center point of the voxel grid. */
	FORCEINLINE FVector GetGridCenter() const { return (BoundsMax + BoundsMin) * 0.5f; }
		
	/**
	 * Returns T value at given location.
	 * @param Location Position to sample on.
	 */
	virtual T GetValueAtLocation(const FVector& Location) const;
	
	/**
	 * Returns T value at given index.
	 * @param Index Index to sample on.
	 */
	virtual T GetValueAtIndex(const int32 Index) const;
	
	/**
	 * Returns voxel center at given indices.
	 * @param IndexX The grid index along the X-axis.
	 * @param IndexY The grid index along the Y-axis.
	 * @param IndexZ The grid index along the Z-axis.
	 */
	FVector GetCellCenterAt(int32 IndexX, int32 IndexY, int32 IndexZ) const;
	
	/**
	 * Returns voxel center at given indices.
	 * @param Index The grid index 
	 */
	FVector GetCellCenterAt(int32 Index) const;
	
	/** Returns voxel cell size. */
	FORCEINLINE FVector GetCellSize() const { return FVector(VoxelSizeX, VoxelSizeY, VoxelSizeZ); }
	
	/** Returns number of voxel grid cells. */
	FORCEINLINE int32 GetGridCellCount() const { return InternalVoxelGrid.Num(); }

	
protected:
	
	/** Initializes the voxel grid. */
	virtual void InitializeVoxelGrid();
	
	/** Calculates the voxel size for each dimension. */
	void CalculateCellSizes();

	/**
	 * Converts 3D location to 1D array index.
	 * @param Location World location to convert.
	 */
	int32 PositionToArrayIndex(const FVector& Location) const;
	
	/**
	 * Converts 3D grid indices to a 1D array index.
	 * @param X The grid index along the X-axis.
	 * @param Y The grid index along the Y-axis.
	 * @param Z The grid index along the Z-axis.
	 */
	FORCEINLINE int32 PositionToArrayIndex(int32 X, int32 Y, int32 Z) const
	{
		return X + Y * DimensionX + Z * DimensionXY;
	}
		
	/**
	 * Computes the voxel X-axis index for a given world location.
	 * @param Location World location to convert.
	 */
	FORCEINLINE int32 CellIndexX(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.X - BoundsMin.X) / VoxelSizeX), 0, DimensionX - 1);
	}

	/**
	 * Computes the voxel Y-axis index for a given world location.
	 * @param Location World location to convert.
	 */
	FORCEINLINE int32 CellIndexY(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Y - BoundsMin.Y) / VoxelSizeY), 0, DimensionY - 1);
	}

	/**
	 * Computes the voxel Z-axis index for a given world location.
	 * @param Location World location to convert.
	 */
	FORCEINLINE int32 CellIndexZ(const FVector& Location) const
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Z - BoundsMin.Z) / VoxelSizeZ), 0, DimensionZ - 1);
	}
	
	
	/** The minimum corner of the voxel grid (left-bottom-back).*/
	FVector BoundsMin;
	/** The maximum corner of the voxel grid (right-top-forward). */
	FVector BoundsMax;
	
	/** Grid resolution along the X axis. */
	int32 DimensionX = 25;
	
	/** Grid resolution along the Y axis. */
	int32 DimensionY = 25;
	
	/** Grid resolution along the Z axis. */
	int32 DimensionZ = 25;
	
	
	/** Precomputed multiplier for XY plane indexing. */
	int32 DimensionXY = DimensionX * DimensionY;
	
	/** Calculated voxel sizes along each axis. */
	float VoxelSizeX = 0.0f;
	float VoxelSizeY = 0.0f;
	float VoxelSizeZ = 0.0f;

	/** Voxel grid storing T data. */
	TArray<T> InternalVoxelGrid;
};

template <class T>
FVoxelGrid<T>::FVoxelGrid(const FVoxelGridData<T>& InVoxelGridData)
{
	const FVector HalfExtent = InVoxelGridData.BoundsData.Extent / 2;
	
	BoundsMin = InVoxelGridData.BoundsData.Center - HalfExtent;
	BoundsMax = InVoxelGridData.BoundsData.Center + HalfExtent;	
	
	DimensionX = InVoxelGridData.BoundsData.GridResolution.X;
	DimensionY = InVoxelGridData.BoundsData.GridResolution.Y;
	DimensionZ = InVoxelGridData.BoundsData.GridResolution.Z;
}

template <class T>
T FVoxelGrid<T>::GetValueAtLocation(const FVector& Location) const
{
	const int32 ArrayIndex = PositionToArrayIndex(Location);
	return InternalVoxelGrid[ArrayIndex];
}

template <class T>
T FVoxelGrid<T>::GetValueAtIndex(const int32 Index) const
{
	const int32 ClampedIndex = FMath::Clamp(Index, 0, InternalVoxelGrid.Num() - 1);
	return InternalVoxelGrid[ClampedIndex];
}

template <class T>
FVector FVoxelGrid<T>::GetCellCenterAt(int32 IndexX, int32 IndexY, int32 IndexZ) const
{
	float LocationX = BoundsMin.X + VoxelSizeX * IndexX + VoxelSizeX / 2;
	float LocationY = BoundsMin.Y + VoxelSizeY * IndexY + VoxelSizeY / 2;
	float LocationZ = BoundsMin.Z + VoxelSizeZ * IndexZ + VoxelSizeZ / 2;

	return FVector(LocationX, LocationY, LocationZ);
}

template <class T>
FVector FVoxelGrid<T>::GetCellCenterAt(int32 Index) const
{
	const int32 ClampedIndex = FMath::Clamp(Index, 0, InternalVoxelGrid.Num() - 1);
	
	const int32 IndexX = ClampedIndex % DimensionX;
	const int32 IndexY = (ClampedIndex / DimensionXY) / DimensionX;
	const int32 IndexZ = ClampedIndex / DimensionXY;
	
	return GetCellCenterAt(IndexX, IndexY, IndexZ);
}

template <class T>
void FVoxelGrid<T>::InitializeVoxelGrid()
{
	InternalVoxelGrid.SetNumZeroed(DimensionX * DimensionY * DimensionZ);
	CalculateCellSizes();
}


template <class T>
void FVoxelGrid<T>::CalculateCellSizes()
{
	VoxelSizeX = (BoundsMax.X - BoundsMin.X) / DimensionX;
	VoxelSizeY = (BoundsMax.Y - BoundsMin.Y) / DimensionY;
	VoxelSizeZ = (BoundsMax.Z - BoundsMin.Z) / DimensionZ;
	
	DimensionXY = DimensionX * DimensionY;
}

template <class T>
int32 FVoxelGrid<T>::PositionToArrayIndex(const FVector& Location) const
{
	const int32 IndexX = CellIndexX(Location);
	const int32 IndexY = CellIndexY(Location);
	const int32 IndexZ = CellIndexZ(Location);
	return PositionToArrayIndex(IndexX, IndexY, IndexZ); 
}
