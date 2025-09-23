// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class BOIDS_PROJECT_API FBounds
{
	
public:
	
	FBounds();
	FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	virtual ~FBounds();

	FORCEINLINE FVector GetMaxCorner() const { return BoundsUpperRightForwardCorner; } 
	FORCEINLINE FVector GetMinCorner() const { return BoundsBottomLeftBackCorner; }
	FORCEINLINE FVector GetBoundsSize() const { return BoundsUpperRightForwardCorner - BoundsBottomLeftBackCorner; }
	FORCEINLINE FVector GetBoundsCenter() const { return (BoundsUpperRightForwardCorner + BoundsBottomLeftBackCorner) / 2; }
	
protected:
	
	FVector BoundsBottomLeftBackCorner;
	FVector BoundsUpperRightForwardCorner;
};
