// Copyright WojtusMientus

// Represents world bounds around the demo world
// Is currently being replaced with WorldCollisionBounds class
// This class is deprecated and will be deleted in future development

#pragma once

#include "CoreMinimal.h"
#include "Bounds/Bounds.h"


class BOIDS_PROJECT_API FWorldBounds: public FBounds
{
	
public:

	// ----- Constructors -----
	FWorldBounds();
	FWorldBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	FWorldBounds(const float BoundsSize);
	~FWorldBounds() = default;

	// ----- Public API -----
	void UpdateBounds(const FVector& Center, const FVector& BoxExtent);
	void WrapPosition(FVector& Position);
};
