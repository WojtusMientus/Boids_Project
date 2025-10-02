// Copyright WojtusMientus

// Base class representing a 3D axis-aligned bounding box.

#pragma once

#include "CoreMinimal.h"


class BOIDS_PROJECT_API FBounds
{
	
public:

	// ----- Constructors ----
	FBounds();
	FBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	virtual ~FBounds() = default;

	// ----- Public API -----
	FORCEINLINE FVector GetMaxCorner() const { return BoundsUpperRightForwardCorner; } 
	FORCEINLINE FVector GetMinCorner() const { return BoundsBottomLeftBackCorner; }
	FORCEINLINE FVector GetBoundsSize() const { return BoundsUpperRightForwardCorner - BoundsBottomLeftBackCorner; }
	FORCEINLINE FVector GetBoundsCenter() const { return (BoundsUpperRightForwardCorner + BoundsBottomLeftBackCorner) / 2; }

protected:
	
	FVector BoundsBottomLeftBackCorner;
	FVector BoundsUpperRightForwardCorner;
};
