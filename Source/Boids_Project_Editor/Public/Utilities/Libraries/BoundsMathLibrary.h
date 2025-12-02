#pragma once


/**
 * Structure for all helper math functions related to bounds calculations.
 */
struct FBoundsMath
{
	
public:
	
	/** Converts 3D indices into linear array index. */
	static FORCEINLINE int32 XYZToArrayIndex(int32 X, int32 Y, int32 Z, const FIntVector& GridResolution)
	{
		return X * GridResolution.Z * GridResolution.Y + Y * GridResolution.Z + Z;
	}
	
	static FORCEINLINE FVector GetVoxelCenterAt(const FVector& StartingCellCenter, const FVector& CellSize, 
		const FIntVector Index)
	{
		const float LocationX = GetCellLocationAxis(StartingCellCenter.X, CellSize.X, Index.X);
		const float LocationY = GetCellLocationAxis(StartingCellCenter.Y, CellSize.Y, Index.Y);
		const float LocationZ = GetCellLocationAxis(StartingCellCenter.Z, CellSize.Z, Index.Z);
		
		return FVector(LocationX, LocationY, LocationZ);
	} 
	
	static FORCEINLINE FVector GetVoxelCenterAt(const FVector& StartingCellCenter, const FVector& CellSize, 
	const int32 IndexX, const int32 IndexY, const int32 IndexZ)
	{
		const float LocationX = GetCellLocationAxis(StartingCellCenter.X, CellSize.X, IndexX);
		const float LocationY = GetCellLocationAxis(StartingCellCenter.Y, CellSize.Y, IndexY);
		const float LocationZ = GetCellLocationAxis(StartingCellCenter.Z, CellSize.Z, IndexZ);
		
		return FVector(LocationX, LocationY, LocationZ);
	} 
	
	static FVector GetVoxelCellSize(const FVector& BoundsCenter, const FVector& Extent, 
		const FIntVector& GridResolution)
	{
		const FVector HalfExtentOfBounds = Extent / 2.0f;
		const FVector MinCorner = BoundsCenter - HalfExtentOfBounds;
		const FVector MaxCorner = BoundsCenter + HalfExtentOfBounds; 
		
		const float CellSizeX = GetCellSizeAxis(MaxCorner.X, MinCorner.X, GridResolution.X);
		const float CellSizeY = GetCellSizeAxis(MaxCorner.Y, MinCorner.Y, GridResolution.Y);
		const float CellSizeZ = GetCellSizeAxis(MaxCorner.Z, MinCorner.Z, GridResolution.Z);
		
		return FVector(CellSizeX, CellSizeY, CellSizeZ);
	}
	
	static FVector GetStartingCellCenter(const FVector& BoundsCenter, const FVector& Extent, 
		const FIntVector& GridResolution)
	{
		const FVector HalfExtentOfBounds = Extent / 2.0f;
		const FVector MinCorner = BoundsCenter - HalfExtentOfBounds;
		
		const FVector CellSize = GetVoxelCellSize(BoundsCenter, Extent, GridResolution);
		
		const float StartingCellCenterX = GetStartingCellCenterAxis(MinCorner.X, CellSize.X);
		const float StartingCellCenterY = GetStartingCellCenterAxis(MinCorner.Y, CellSize.Y);
		const float StartingCellCenterZ = GetStartingCellCenterAxis(MinCorner.Z, CellSize.Z);
		
		return FVector(StartingCellCenterX, StartingCellCenterY, StartingCellCenterZ);
	}
	
	static FORCEINLINE bool IsInBounds(const FIntVector GridResolution, const FIntVector Index)
	{
		return IsInBoundsAxis(GridResolution.X, Index.X) && 
				IsInBoundsAxis(GridResolution.Y, Index.Y) && 
				IsInBoundsAxis(GridResolution.Z, Index.Z);
	}
	
private:
	
	static float GetCellLocationAxis(const float Start, const float CellSize, const float Index)
	{
		return Start + CellSize * Index;
	}
	
	static float GetCellSizeAxis(const float MaxAxis, const float MinAxis, const float AxisResolution)
	{
		return (MaxAxis - MinAxis) / AxisResolution;
	}
	
	static float GetStartingCellCenterAxis(const float Min, const float Size)
	{
		return Min + Size / 2.0f;
	}	
	
	static FORCEINLINE bool IsInBoundsAxis(const int32 AxisMax, const int32 Index)
	{
		return Index >= 0 && Index < AxisMax;
	} 
};
