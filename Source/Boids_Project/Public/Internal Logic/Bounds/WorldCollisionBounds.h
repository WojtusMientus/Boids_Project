// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Internal Logic/Bounds/Bounds.h"


class BOIDS_PROJECT_API FWorldCollisionBounds: public FBounds
{
public:
	
	FWorldCollisionBounds();
	FWorldCollisionBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	FWorldCollisionBounds(const float BoundsSize);
	~FWorldCollisionBounds();

	void UpdateBounds(const FVector& Center, const FVector& BoxExtent);
	
	FVector GetCollisionForceAt(const FVector& Location);
	void AddForceAtIndex(const FVector& Force, const int32 Index);
	void InitializeForcesNearBounds();

	FVector GetBoundCellCenter(int32 X, int32 Y, int32 Z);

	static constexpr int32 DIMENSION_X = 35;
	static constexpr int32 DIMENSION_Y = 35;
	static constexpr int32 DIMENSION_Z = 35;
	
protected:
		

	static constexpr int32 DIMENSION_XY = DIMENSION_X * DIMENSION_Y;
	static constexpr int32 COLLISION_ROWS = 3;
	
	float CELL_SIZE_X;
	float CELL_SIZE_Y;
	float CELL_SIZE_Z;

	static constexpr float COLLISION_MULTIPLIER = .2;
	

	void UpdateCellSizes();
	
	FORCEINLINE int32 ToArrayIndex(int32 X, int32 Y, int32 Z)
	{
		return X + Y * DIMENSION_X + Z * DIMENSION_XY;
	}

	FORCEINLINE int32 CellIndexX(const FVector& Location)
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.X - BoundsBottomLeftBackCorner.X) / CELL_SIZE_X), 0, DIMENSION_X - 1);
	}

	FORCEINLINE int32 CellIndexY(const FVector& Location)
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Y - BoundsBottomLeftBackCorner.Y) / CELL_SIZE_Y), 0, DIMENSION_Y - 1);
	}

	FORCEINLINE int32 CellIndexZ(const FVector& Location)
	{
		return FMath::Clamp(FMath::FloorToInt32((Location.Z - BoundsBottomLeftBackCorner.Z) / CELL_SIZE_Z), 0, DIMENSION_Z - 1);
	}
	
	TArray<FVector> VoxelCollisionGrid;	
};
