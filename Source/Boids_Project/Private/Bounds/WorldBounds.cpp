
#include "Bounds/WorldBounds.h"


FWorldBounds::FWorldBounds(): FBounds()
{
}

FWorldBounds::FWorldBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
}

FWorldBounds::FWorldBounds(const float BoundsSize)
{
	const float HalfBoundsSize = BoundsSize / 2;
	BoundsMax = FVector(HalfBoundsSize, HalfBoundsSize, HalfBoundsSize);
	BoundsMin = FVector(-HalfBoundsSize, -HalfBoundsSize, -HalfBoundsSize);
}

void FWorldBounds::UpdateBounds(const FVector& Center, const FVector& BoxExtent)
{
	const FVector HalfExtent = BoxExtent / 2;

	BoundsMin = Center - HalfExtent;
	BoundsMax = Center + HalfExtent;
}

void FWorldBounds::WrapPosition(FVector& Position)
{
	const FVector MinCorner = GetMinCorner();
	const FVector MaxCorner = GetMaxCorner();

	Position.X = FMath::Wrap(Position.X, MinCorner.X, MaxCorner.X);
	Position.Y = FMath::Wrap(Position.Y, MinCorner.Y, MaxCorner.Y);
	Position.Z = FMath::Wrap(Position.Z, MinCorner.Z, MaxCorner.Z);
}
