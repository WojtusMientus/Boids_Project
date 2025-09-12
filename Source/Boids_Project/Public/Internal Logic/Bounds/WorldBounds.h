// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Internal Logic/Bounds/Bounds.h"


class BOIDS_PROJECT_API FWorldBounds: public FBounds
{
public:
	
	FWorldBounds();
	FWorldBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	FWorldBounds(const float BoundsSize);
	~FWorldBounds();

	void UpdateBounds(const FVector& Center, const FVector& BoxExtent);
	void WrapPosition(FVector& Position);
	
};
