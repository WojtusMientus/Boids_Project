
#include "Bounds/Bounds.h"


FBounds::FBounds()
{
}

FBounds::FBounds(const FVector& MaxCorner, const FVector& MinCorner): BoundsMin(MinCorner), BoundsMax(MaxCorner)
{
}
