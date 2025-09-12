// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BOIDS_PROJECT_API FBounds
{
	
public:
	
	FBounds();
	FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	~FBounds();

	bool IsInBounds(const FVector& Position);
	void UpdateBounds(const FVector& Center, const FVector& BoxExtent);
	
	FVector BoundsBottomLeftBackCorner;
	FVector BoundsUpperRightForwardCorner;

private:
	bool IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck);

	bool IsInAxesX(float CoordinateToCheck);
	bool IsInAxesY(float CoordinateToCheck);
	bool IsInAxesZ(float CoordinateToCheck);
	
};
