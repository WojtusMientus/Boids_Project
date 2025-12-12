// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"
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
	FORCEINLINE FVector GetExtent() const { return BoundsMax - BoundsMin; }

	/** Returns the center point of the voxel grid. */
	FORCEINLINE FVector GetGridCenter() const { return (BoundsMax + BoundsMin) * 0.5f; }
		
	/**
	 * Returns T value at given location.
	 * @param Location Position to sample on.
	 */
	FORCEINLINE T GetValueAtLocation(const FVector& Location) const;
	
	/**
	 * Returns T value at given index.
	 * @param Index Index to sample on.
	 */
	FORCEINLINE T GetValueAtIndex(const int32 Index) const;
	
	/** Returns number of voxel grid cells. */
	FORCEINLINE int32 GetGridCellCount() const { return InternalVoxelGrid.Num(); }

	
protected:
	
	/** Initializes the voxel grid. */
	virtual void InitializeVoxelGrid();
	
	/**
	 * Converts 3D location to 1D array index.
	 * @param Location World location to convert.
	 */
	int32 FORCEINLINE PositionToArrayIndex(const FVector& Location) const;
	
	/**
	 * Converts 3D grid indices to a 1D array index.
	 * @param X The grid index along the X-axis.
	 * @param Y The grid index along the Y-axis.
	 * @param Z The grid index along the Z-axis.
	 */
	FORCEINLINE int32 PositionToArrayIndex(int32 X, int32 Y, int32 Z) const
	{
		return FBoundsMath::XYZToArrayIndex(X, Y, Z, GridResolution);
	}
		
	
	/** The minimum corner of the voxel grid (left-bottom-back).*/
	FVector BoundsMin;
	
	/** The maximum corner of the voxel grid (right-top-forward). */
	FVector BoundsMax;
	
	/** Grid resolution along the axes. */
	FIntVector GridResolution = FIntVector(25,25,25);
	
	/** Calculated voxel sizes along each axis. */
	FVector VoxelCellSize = FVector();
	
	/** Voxel grid storing T data. */
	TArray<T> InternalVoxelGrid;
};

template <class T>
FVoxelGrid<T>::FVoxelGrid(const FVoxelGridData<T>& InVoxelGridData)
{
	const FVector HalfExtent = InVoxelGridData.BoundsData.Extent / 2;
	
	BoundsMin = InVoxelGridData.BoundsData.Center - HalfExtent;
	BoundsMax = InVoxelGridData.BoundsData.Center + HalfExtent;	
	
	GridResolution = InVoxelGridData.BoundsData.GridResolution;
}

template <class T>
T FVoxelGrid<T>::GetValueAtLocation(const FVector& Location) const
{
	const int32 ClampedArrayIndex = FMath::Clamp(PositionToArrayIndex(Location), 
		0, InternalVoxelGrid.Num() - 1);
	return InternalVoxelGrid[ClampedArrayIndex];
}

template <class T>
T FVoxelGrid<T>::GetValueAtIndex(const int32 Index) const
{
	const int32 ClampedIndex = FMath::Clamp(Index, 0, InternalVoxelGrid.Num() - 1);
	return InternalVoxelGrid[ClampedIndex];
}

template <class T>
void FVoxelGrid<T>::InitializeVoxelGrid()
{
	InternalVoxelGrid.SetNumZeroed(GridResolution.X * GridResolution.Y * GridResolution.Z);
	VoxelCellSize = FBoundsMath::GetVoxelCellSize(GetGridCenter(), GetExtent(), GridResolution);
}

template <class T>
int32 FVoxelGrid<T>::PositionToArrayIndex(const FVector& Location) const
{
	FIntVector ArrayCoordinates = FBoundsMath::ClampToNearestBoundIndices(Location, BoundsMin, 
		VoxelCellSize, GridResolution);
	return FBoundsMath::XYZToArrayIndex(ArrayCoordinates, GridResolution); 
}
