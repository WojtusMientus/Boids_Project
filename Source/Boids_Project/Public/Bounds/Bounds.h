// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"

/**
 * Base Bounds class representing a 3D axis-aligned bounding box. (AABB)
 */
class BOIDS_PROJECT_API FBounds
{
	
public:

	/** Constructs a bounds from the given min and max corners.  */
	FBounds(const FVector& MaxCorner = FVector(), const FVector& MinCorner = FVector());
	
	/** Returns the maximum corner of the box. */
	FORCEINLINE const FVector& GetMax() const { return BoundsMax; }

	/** Returns the minimum corner of the box. */
	FORCEINLINE const FVector& GetMin() const { return BoundsMin; }

	/** Returns the size of the box (Max - Min). */
	FORCEINLINE FVector GetSize() const { return BoundsMax - BoundsMin; }

	/** Returns the center point of the box. */
	FORCEINLINE FVector GetCenter() const { return (BoundsMax + BoundsMin) * 0.5f; }

protected:

	/** The minimum corner of the box (left-bottom-back).*/
	FVector BoundsMin;
	/** The maximum corner of the box (right-top-forward). */
	FVector BoundsMax;
};
