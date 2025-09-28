
#include "Bounds/Bounds.h"


FBounds::FBounds()
{
}

FBounds::FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner):
BoundsBottomLeftBackCorner(BottomBackLeftCorner), BoundsUpperRightForwardCorner(UpperForwardRightCorner)
{
}
