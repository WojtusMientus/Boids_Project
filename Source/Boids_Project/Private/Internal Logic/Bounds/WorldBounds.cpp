// Fill out your copyright notice in the Description page of Project Settings.


#include "Internal Logic/Bounds/WorldBounds.h"


FWorldBounds::FWorldBounds(): FBounds()
{
}

FWorldBounds::FWorldBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner): FBounds(UpperForwardRightCorner, BottomBackLeftCorner)
{
}

FWorldBounds::FWorldBounds(const float BoundsSize)
{
	const float HalfBoundsSize = BoundsSize / 2;
	BoundsUpperRightForwardCorner = FVector(HalfBoundsSize, HalfBoundsSize, HalfBoundsSize);
	BoundsBottomLeftBackCorner = FVector(-HalfBoundsSize, -HalfBoundsSize, -HalfBoundsSize);
}

FWorldBounds::~FWorldBounds()
{
}

void FWorldBounds::UpdateBounds(const FVector& Center, const FVector& BoxExtent)
{
	const FVector HalfExtent = BoxExtent / 2;

	BoundsBottomLeftBackCorner = Center - HalfExtent;
	BoundsUpperRightForwardCorner = Center + HalfExtent;
}

void FWorldBounds::WrapPosition(FVector& Position)
{
	const FVector BoundsSize = GetBoundsSize();
	const FVector MinCorner = GetMinCorner();
	const FVector MaxCorner = GetMaxCorner();

	if (Position.X >= MaxCorner.X)
	{
		Position.X -= BoundsSize.X;
	}
	else if (Position.X < MinCorner.X)
	{
		Position.X += BoundsSize.X;
	}

	if (Position.Y >= MaxCorner.Y)
	{
		Position.Y -= BoundsSize.Y;
	}
	else if (Position.Y < MinCorner.Y)
	{
		Position.Y += BoundsSize.Y;
	}

	if (Position.Z >= MaxCorner.Z)
	{
		Position.Z -= BoundsSize.Z;
	}
	else if (Position.Z < MinCorner.Z)
	{
		Position.Z += BoundsSize.Z;
	}
}
