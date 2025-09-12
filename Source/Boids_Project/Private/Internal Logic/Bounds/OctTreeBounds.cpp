// Fill out your copyright notice in the Description page of Project Settings.


#include "Internal Logic/Bounds/OctTreeBounds.h"


FOctTreeBounds::FOctTreeBounds(): FBounds()
{
}

FOctTreeBounds::FOctTreeBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
}

FOctTreeBounds::~FOctTreeBounds()
{
}

bool FOctTreeBounds::IsInBounds(const FVector& Position)
{
	return IsInAxesX(Position.X) && IsInAxesY(Position.Y) && IsInAxesZ(Position.Z);
}

// I made it intentionally min inclusive and max exclusive to prevent adding point into multiple OctTree leaves in the end
// It is not the perfect solution, but this way I have guarantee it will work as intended

bool FOctTreeBounds::IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck)
{
	return MinBound <= CoordinateToCheck && CoordinateToCheck < MaxBound;
}

bool FOctTreeBounds::IsInAxesX(float CoordinateToCheck)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.X, BoundsUpperRightForwardCorner.X, CoordinateToCheck);
}

bool FOctTreeBounds::IsInAxesY(float CoordinateToCheck)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.Y, BoundsUpperRightForwardCorner.Y, CoordinateToCheck);
}

bool FOctTreeBounds::IsInAxesZ(float CoordinateToCheck)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.Z, BoundsUpperRightForwardCorner.Z, CoordinateToCheck);
}