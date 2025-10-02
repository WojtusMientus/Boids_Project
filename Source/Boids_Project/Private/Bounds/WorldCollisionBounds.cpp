
#include "Bounds/WorldCollisionBounds.h"


FWorldCollisionBounds::FWorldCollisionBounds(): FBounds()
{
	VoxelCollisionGrid.SetNumZeroed(DIMENSION_X * DIMENSION_Y * DIMENSION_Z);
	InitializeCellSizes();
	InitializeForcesNearBounds();
}

FWorldCollisionBounds::FWorldCollisionBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
	VoxelCollisionGrid.SetNumZeroed(DIMENSION_X * DIMENSION_Y * DIMENSION_Z);
	InitializeCellSizes();
	InitializeForcesNearBounds();
}

FWorldCollisionBounds::FWorldCollisionBounds(const float BoundsSize)
{
	const float HalfBoundsSize = BoundsSize / 2;
	BoundsMax = FVector(HalfBoundsSize, HalfBoundsSize, HalfBoundsSize);
	BoundsMin = FVector(-HalfBoundsSize, -HalfBoundsSize, -HalfBoundsSize);

	VoxelCollisionGrid.SetNumZeroed(DIMENSION_X * DIMENSION_Y * DIMENSION_Z);
	InitializeCellSizes();
	InitializeForcesNearBounds();
}

void FWorldCollisionBounds::UpdateBounds(const FVector& Center, const FVector& BoxExtent)
{
	const FVector HalfExtent = BoxExtent / 2;

	BoundsMin = Center - HalfExtent;
	BoundsMax = Center + HalfExtent;
	InitializeCellSizes();
}

FVector FWorldCollisionBounds::GetCollisionForceAt(const FVector& Location) const
{
	const int32 IndexX = CellIndexX(Location);
	const int32 IndexY = CellIndexY(Location);
	const int32 IndexZ = CellIndexZ(Location);
	const int32 ArrayIndex = ToArrayIndex(IndexX, IndexY, IndexZ); 
		
	return VoxelCollisionGrid[ArrayIndex] * COLLISION_MULTIPLIER;
}

FVector FWorldCollisionBounds::GetCellCenter(int32 IndexX, int32 IndexY, int32 IndexZ) const
{
	float LocationX = BoundsMin.X + CellSizeX * IndexX + CellSizeX / 2;
	float LocationY = BoundsMin.Y + CellSizeY * IndexY + CellSizeY / 2;
	float LocationZ = BoundsMin.Z + CellSizeZ * IndexZ + CellSizeZ / 2;

	return FVector(LocationX, LocationY, LocationZ);
}

void FWorldCollisionBounds::InitializeForcesNearBounds()
{
	InitializeForcesAlongX(0, COLLISION_ROWS, FVector::ForwardVector, true);
	InitializeForcesAlongX(DIMENSION_X - COLLISION_ROWS, DIMENSION_X, FVector::BackwardVector, false);
	
	InitializeForcesAlongY(0, COLLISION_ROWS, FVector::RightVector, true);
	InitializeForcesAlongY(DIMENSION_Y - COLLISION_ROWS, DIMENSION_Y, FVector::LeftVector, false);
	
	InitializeForcesAlongZ(0, COLLISION_ROWS, FVector::UpVector, true);
	InitializeForcesAlongZ(DIMENSION_Z - COLLISION_ROWS, DIMENSION_Z, FVector::DownVector, false);
}

void FWorldCollisionBounds::InitializeForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary)
{
	for (int x = StartIndex; x < EndIndex; x++)
	{
		int ForceMagnitude = DIMENSION_X - x;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = COLLISION_ROWS - x;
		}
		
		for (int y = 0; y < DIMENSION_Y; y++)
		{
			for (int z = 0; z < DIMENSION_Z; z++)
			{
				const int32 Index = ToArrayIndex(x, y, z);
				AddForceAt(ForceVector * ForceMagnitude, Index);
			}
		}
	}
}

void FWorldCollisionBounds::InitializeForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary)
{
	for (int y = StartIndex; y < EndIndex; y++)
	{
		int ForceMagnitude = DIMENSION_Y - y;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = COLLISION_ROWS - y;
		}
		
		for (int x = 0; x < DIMENSION_X; x++)
		{
			for (int z = 0; z < DIMENSION_Z; z++)
			{
				const int32 Index = ToArrayIndex(x, y, z);
				AddForceAt(ForceVector * ForceMagnitude, Index);
			}
		}
	}
}

void FWorldCollisionBounds::InitializeForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary)
{
	for (int z = StartIndex; z < EndIndex; z++)
	{
		int ForceMagnitude = DIMENSION_Z - z;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = COLLISION_ROWS - z;
		}
		
		for (int x = 0; x < DIMENSION_X; x++)
		{
			for (int y = 0; y < DIMENSION_Y; y++)
			{
				const int32 Index = ToArrayIndex(x, y, z);
				AddForceAt(ForceVector * ForceMagnitude, Index);
			}
		}
	}
}

void FWorldCollisionBounds::InitializeCellSizes()
{
	CellSizeX = (BoundsMax.X - BoundsMin.X) / DIMENSION_X;
	CellSizeY = (BoundsMax.Y - BoundsMin.Y) / DIMENSION_Y;
	CellSizeZ = (BoundsMax.Z - BoundsMin.Z) / DIMENSION_Z;
}

void FWorldCollisionBounds::AddForceAt(const FVector& Force, int32 Index)
{
	VoxelCollisionGrid[Index] += Force;
}