// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Internal Logic/Bounds/Bounds.h"


class BOIDS_PROJECT_API FOctTreeBounds: public FBounds
{
public:
	FOctTreeBounds();
	FOctTreeBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	~FOctTreeBounds();

	bool IsInBounds(const FVector& Position);

	bool IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck);

	bool IsInAxesX(float CoordinateToCheck);
	bool IsInAxesY(float CoordinateToCheck);
	bool IsInAxesZ(float CoordinateToCheck);
	

	
};
