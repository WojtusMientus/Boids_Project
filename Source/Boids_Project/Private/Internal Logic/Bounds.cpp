// Fill out your copyright notice in the Description page of Project Settings.


#include "Internal Logic/Bounds.h"

FBounds::FBounds()
{
}

FBounds::FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner)
{
	BoundsBottomLeftBackCorner = BottomBackLeftCorner;
	BoundsUpperRightForwardCorner = UpperForwardRightCorner;
}

FBounds::~FBounds()
{
}

bool FBounds::IsInBounds(const FVector& Position)
{
	return IsInAxesX(Position.X) && IsInAxesY(Position.Y) && IsInAxesZ(Position.Z);
}

void FBounds::UpdateBounds(const FVector& Center, const FVector& BoxExtent)
{
	const FVector HalfExtent = BoxExtent / 2;

	BoundsBottomLeftBackCorner = Center - HalfExtent;
	BoundsUpperRightForwardCorner = Center + HalfExtent;
}

// Making this way intentionally, since I will want to include this Bounds class in my OctTree later in the development.
// I made it min inclusive and max exclusive to prevent adding point into multiple OctTree leaves in the end
// It is not the perfect solution, but this way I have guarantee it will work as intended

bool FBounds::IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck)
{
	return MinBound <= CoordinateToCheck && CoordinateToCheck < MaxBound;
}

bool FBounds::IsInAxesX(float CoordinateToCheck)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.X, BoundsUpperRightForwardCorner.X, CoordinateToCheck);
}

bool FBounds::IsInAxesY(float CoordinateToCheck)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.Y, BoundsUpperRightForwardCorner.Y, CoordinateToCheck);
}

bool FBounds::IsInAxesZ(float CoordinateToCheck)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.Z, BoundsUpperRightForwardCorner.Z, CoordinateToCheck);
}

