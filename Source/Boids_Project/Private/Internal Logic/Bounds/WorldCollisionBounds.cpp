// Fill out your copyright notice in the Description page of Project Settings.


#include "Internal Logic/Bounds/WorldCollisionBounds.h"

#include "DataWrappers/ChaosVDJointDataWrappers.h"


FWorldCollisionBounds::FWorldCollisionBounds(): FBounds()
{
	VoxelCollisionGrid.SetNumZeroed(DIMENSION_X * DIMENSION_Y * DIMENSION_Z);
	UpdateCellSizes();
	InitializeForcesNearBounds();
}

FWorldCollisionBounds::FWorldCollisionBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
	VoxelCollisionGrid.SetNumZeroed(DIMENSION_X * DIMENSION_Y * DIMENSION_Z);
	UpdateCellSizes();
	InitializeForcesNearBounds();
}

FWorldCollisionBounds::FWorldCollisionBounds(const float BoundsSize)
{
	const float HalfBoundsSize = BoundsSize / 2;
	BoundsUpperRightForwardCorner = FVector(HalfBoundsSize, HalfBoundsSize, HalfBoundsSize);
	BoundsBottomLeftBackCorner = FVector(-HalfBoundsSize, -HalfBoundsSize, -HalfBoundsSize);

	VoxelCollisionGrid.SetNumZeroed(DIMENSION_X * DIMENSION_Y * DIMENSION_Z);
	UpdateCellSizes();
	InitializeForcesNearBounds();
}

FWorldCollisionBounds::~FWorldCollisionBounds()
{
}

void FWorldCollisionBounds::UpdateBounds(const FVector& Center, const FVector& BoxExtent)
{
	const FVector HalfExtent = BoxExtent / 2;

	BoundsBottomLeftBackCorner = Center - HalfExtent;
	BoundsUpperRightForwardCorner = Center + HalfExtent;
	UpdateCellSizes();
}

FVector FWorldCollisionBounds::GetCollisionForceAt(const FVector& Location)
{
	const int32 IndexX = CellIndexX(Location);
	const int32 IndexY = CellIndexY(Location);
	const int32 IndexZ = CellIndexZ(Location);
	const int32 ArrayIndex = ToArrayIndex(IndexX, IndexY, IndexZ); 
		
	return VoxelCollisionGrid[ArrayIndex] * COLLISION_MULTIPLIER;
}

void FWorldCollisionBounds::AddForceAtIndex(const FVector& Force, const int32 Index)
{
	VoxelCollisionGrid[Index] += Force;
}

void FWorldCollisionBounds::InitializeForcesNearBounds()
{
	for (int x = 0; x < COLLISION_ROWS; x++)
	{
		for (int y = 0; y < DIMENSION_Y; y++)
		{
			for (int z = 0; z < DIMENSION_Z; z++)
			{
				const int32 ArrayIndex = ToArrayIndex(x, y, z);
				AddForceAtIndex(FVector::ForwardVector * (COLLISION_ROWS - x), ArrayIndex);
			}
		}
	}

	for (int x = DIMENSION_X - COLLISION_ROWS; x < DIMENSION_X; x++)
	{
		for (int y = 0; y < DIMENSION_Y; y++)
		{
			for (int z = 0; z < DIMENSION_Z; z++)
			{
				const int32 ArrayIndex = ToArrayIndex(x, y, z);
				AddForceAtIndex(FVector::BackwardVector * (DIMENSION_X - x), ArrayIndex);
			}
		}
	}


	for (int y = 0; y < COLLISION_ROWS; y++)
	{
		for (int x = 0; x < DIMENSION_X; x++)
		{
			for (int z = 0; z < DIMENSION_Z; z++)
			{
				const int32 ArrayIndex = ToArrayIndex(x, y, z);
				AddForceAtIndex(FVector::RightVector * (COLLISION_ROWS - y), ArrayIndex);
			}
		}
	}

	for (int y = DIMENSION_Y - COLLISION_ROWS; y < DIMENSION_Y; y++)
	{
		for (int x = 0; x < DIMENSION_X; x++)
		{
			for (int z = 0; z < DIMENSION_Z; z++)
			{
				const int32 ArrayIndex = ToArrayIndex(x, y, z);
				AddForceAtIndex(FVector::LeftVector * (DIMENSION_Y - y), ArrayIndex);
			}
		}
	}


	for (int z = 0; z < COLLISION_ROWS; z++)
	{
		for (int y = 0; y < DIMENSION_Y; y++)
		{
			for (int x = 0; x < DIMENSION_X; x++)
			{
				const int32 ArrayIndex = ToArrayIndex(x, y, z);
				AddForceAtIndex(FVector::UpVector * (COLLISION_ROWS - z), ArrayIndex);
			}
		}
	}

	for (int z = DIMENSION_Z - COLLISION_ROWS; z < DIMENSION_Z; z++)
	{
		for (int y = 0; y < DIMENSION_Y; y++)
		{
			for (int x = 0; x < DIMENSION_X; x++)
			{
				const int32 ArrayIndex = ToArrayIndex(x, y, z);
				AddForceAtIndex(FVector::DownVector * (DIMENSION_Z - z), ArrayIndex);
			}
		}
	}
}

FVector FWorldCollisionBounds::GetBoundCellCenter(int32 IndexX, int32 IndexY, int32 IndexZ)
{
	float LocationX = BoundsBottomLeftBackCorner.X + CELL_SIZE_X * IndexX + CELL_SIZE_X / 2;
	float LocationY = BoundsBottomLeftBackCorner.Y + CELL_SIZE_Y * IndexY + CELL_SIZE_Y / 2;
	float LocationZ = BoundsBottomLeftBackCorner.Z + CELL_SIZE_Z * IndexZ + CELL_SIZE_Z / 2;

	return FVector(LocationX, LocationY, LocationZ);
}

void FWorldCollisionBounds::UpdateCellSizes()
{
	CELL_SIZE_X = (BoundsUpperRightForwardCorner.X - BoundsBottomLeftBackCorner.X) / DIMENSION_X;
	CELL_SIZE_Y = (BoundsUpperRightForwardCorner.Y - BoundsBottomLeftBackCorner.Y) / DIMENSION_Y;
	CELL_SIZE_Z = (BoundsUpperRightForwardCorner.Z - BoundsBottomLeftBackCorner.Z) / DIMENSION_Z;
}
