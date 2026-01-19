// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"


/**
 * Helper struct used by BoidCollisionVoxelGrid for boid to boid collision calculation.
 */
struct FBoidCollisionCellData
{
	uint8 SpeciesIndex = INDEX_NONE;
	uint16 BoidIndex = INDEX_NONE;
	
	
	FBoidCollisionCellData() {}
	FBoidCollisionCellData(const uint8 SpeciesID, const uint16 BoidID): SpeciesIndex(SpeciesID), BoidIndex(BoidID) {}
};
