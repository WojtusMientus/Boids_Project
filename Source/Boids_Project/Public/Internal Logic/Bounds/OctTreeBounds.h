
// First iteration of octree nodes; checks if a point is inside.
// Min-inclusive, max-exclusive bounds to avoid multiple leaf assignments.
// This class will be deprecated and will be deleted in future development
// Collision checks will be moved to VoxelGrid instead of OctTree

#pragma once

#include "CoreMinimal.h"
#include "Internal Logic/Bounds/Bounds.h"


class BOIDS_PROJECT_API FOctTreeBounds: public FBounds
{
	
public:

	// ----- Constructors -----
	FOctTreeBounds();
	FOctTreeBounds(const FVector& UpperForwardRightCorner, const FVector& BottomBackLeftCorner);
	~FOctTreeBounds() = default;

	// ----- Public API -----
	bool IsInBounds(const FVector& Position);

private:

	// ----- Private Helpers -----
	bool IsInAxisRange(float MinBound, float MaxBound, float CoordinateToCheck);
};
