// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "Utilities/ObjectPools/BoidPool.h"

class FBoid;
struct FBoidsSpeciesPlainInfo;


/** 
 * Runtime representation of a boid species, containing its boid pool and species-wide simulation data.
 */
struct FBoidSpecies
{
	FBoidPool BoidPool;
	FBoidsSpeciesPlainInfo SpeciesInfo;
	
	
	FBoidSpecies(const FBoidsSpeciesPlainInfo& SpeciesInfo);
	
	/** Boid pool wrappers. */
	void AddBoids(const int32 CountToAdd);
	void RemoveBoids(const int32 CountToRemove);
	
	int32 Num() const;

	
	void UpdateBoidsForceParameters(float NewSeparationForce, float NewAlignmentForce, float NewCohesionForce,
		float NewOtherSpeciesMultiplier);
	void UpdateBoidsSpatialAwarenessParameters(float NewDesiredSpeed, float NewPerceptionDistance);
	void UpdateBoidCollisionMultiplier(float NewEnvironmentCollisionMultiplier,	float NewBoundsCollisionMultiplier);
};
