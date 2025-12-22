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
	
	virtual ~FVoxelGrid() = default;
	
	/** Returns the size of the voxel grid (Max - Min). */
	FORCEINLINE FVector GetExtent() const { return BoundsMax - BoundsMin; }

	/** Returns the center point of the voxel grid. */
	FORCEINLINE FVector GetGridCenter() const { return (BoundsMax + BoundsMin) * 0.5f; }
		
	/**
	 * Returns T value at given location.
	 * @param Location Position to sample on.
	 */
	FORCEINLINE const T& GetValueAtLocation(const FVector& Location) const;
	
	/**
	 * Returns T value at given index.
	 * @param Index Index to sample on.
	 */
	FORCEINLINE const T& GetValueAtIndex(const int32 Index) const;
	
	/** Returns number of voxel grid cells. */
	FORCEINLINE int32 GetGridCellCount() const { return InternalVoxelGrid.Num(); }

	
	FORCEINLINE FVector GetRandomPointInsideGrid() const;
	
protected:
	
	/** Initializes the voxel grid. */
	virtual void InitializeVoxelGrid(const FVoxelGridData<T>& InVoxelGridData);
	
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
	FVector BoundsMin = FVector::Zero();
	
	/** The maximum corner of the voxel grid (right-top-forward). */
	FVector BoundsMax = FVector::Zero();
	
	/** Grid resolution along the axes. */
	FIntVector GridResolution = FIntVector(10,10,10);
	
	/** Calculated voxel sizes along each axis. */
	FVector VoxelCellSize = FVector();
	
	/** Voxel grid storing T data. */
	TArray<T> InternalVoxelGrid;
};

template <class T>
const T& FVoxelGrid<T>::GetValueAtLocation(const FVector& Location) const
{
	const T& Value = GetValueAtIndex(PositionToArrayIndex(Location));
	return Value;
}

template <class T>
const T& FVoxelGrid<T>::GetValueAtIndex(const int32 Index) const
{
	const int32 ClampedIndex = FMath::Clamp(Index, 0, InternalVoxelGrid.Num() - 1);
	return InternalVoxelGrid[ClampedIndex];
}

template <class T>
FVector FVoxelGrid<T>::GetRandomPointInsideGrid() const
{
	return FVector(FMath::RandRange(BoundsMin.X, BoundsMax.X),
			FMath::RandRange(BoundsMin.Y, BoundsMax.Y),
			FMath::RandRange(BoundsMin.Z, BoundsMax.Z));
}

template <class T>
void FVoxelGrid<T>::InitializeVoxelGrid(const FVoxelGridData<T>& InVoxelGridData)
{
	const FVector HalfExtent = InVoxelGridData.BoundsData.Extent / 2;
	
	BoundsMin = InVoxelGridData.BoundsData.Center - HalfExtent;
	BoundsMax = InVoxelGridData.BoundsData.Center + HalfExtent;	
	
	GridResolution = InVoxelGridData.BoundsData.GridResolution;
	VoxelCellSize = FBoundsMath::GetVoxelCellSize(InVoxelGridData.BoundsData.Center, 
		InVoxelGridData.BoundsData.Extent, GridResolution);
	
	InternalVoxelGrid = InVoxelGridData.VoxelGridData;
}

template <class T>
int32 FVoxelGrid<T>::PositionToArrayIndex(const FVector& Location) const
{
	FIntVector ArrayCoordinates = FBoundsMath::ClampToNearestBoundIndices(Location, BoundsMin, 
		VoxelCellSize, GridResolution);
	return FBoundsMath::XYZToArrayIndex(ArrayCoordinates, GridResolution); 
}
