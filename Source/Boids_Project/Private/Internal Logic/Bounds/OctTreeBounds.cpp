#include "Internal Logic/Bounds/OctTreeBounds.h"


FOctTreeBounds::FOctTreeBounds(): FBounds()
{
}

FOctTreeBounds::FOctTreeBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
}


bool FOctTreeBounds::IsInBounds(const FVector& Position)
{
	return IsInAxisRange(BoundsBottomLeftBackCorner.X, BoundsUpperRightForwardCorner.X, Position.X) &&
			IsInAxisRange(BoundsBottomLeftBackCorner.Y, BoundsUpperRightForwardCorner.Y, Position.Y) &&
			IsInAxisRange(BoundsBottomLeftBackCorner.Z, BoundsUpperRightForwardCorner.Z, Position.Z);
}

// The bounds are intentionally min-inclusive and max-exclusive to ensure that points on the boundary are only assigned to one octree leaf.
bool FOctTreeBounds::IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck)
{
	return MinBound <= CoordinateToCheck && CoordinateToCheck < MaxBound;
}

