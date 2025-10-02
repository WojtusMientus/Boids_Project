
#include "Bounds/OctTreeBounds.h"


FOctTreeBounds::FOctTreeBounds(): FBounds()
{
}

FOctTreeBounds::FOctTreeBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
}


bool FOctTreeBounds::IsInBounds(const FVector& Position)
{
	return IsInAxisRange(BoundsMin.X, BoundsMax.X, Position.X) &&
			IsInAxisRange(BoundsMin.Y, BoundsMax.Y, Position.Y) &&
			IsInAxisRange(BoundsMin.Z, BoundsMax.Z, Position.Z);
}

// The bounds are intentionally min-inclusive and max-exclusive to ensure that points on the boundary are only assigned to one octree leaf.
bool FOctTreeBounds::IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck)
{
	return MinBound <= CoordinateToCheck && CoordinateToCheck < MaxBound;
}

