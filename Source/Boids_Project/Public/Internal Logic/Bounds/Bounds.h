// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class BOIDS_PROJECT_API FBounds
{
	
public:
	
	FBounds();
	FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	virtual ~FBounds();

	FORCEINLINE FVector GetMaxCorner() { return BoundsUpperRightForwardCorner; } 
	FORCEINLINE FVector GetMinCorner() { return BoundsBottomLeftBackCorner; }
	FORCEINLINE FVector GetBoundsSize() { return BoundsUpperRightForwardCorner - BoundsBottomLeftBackCorner; }
	FORCEINLINE FVector GetBoundsCenter() { return (BoundsUpperRightForwardCorner + BoundsBottomLeftBackCorner) / 2; }
	
protected:
	
	FVector BoundsBottomLeftBackCorner;
	FVector BoundsUpperRightForwardCorner;
};
