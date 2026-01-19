
#include "Core/BoidSpecies.h"
#include "Utilities/ObjectPools/BoidPool.h"


FBoidSpecies::FBoidSpecies(const FBoidsSpeciesPlainInfo& SpeciesInfo): 
BoidPool(SpeciesInfo.Number), SpeciesInfo(SpeciesInfo)
{
	AddBoids(SpeciesInfo.Number);
}

void FBoidSpecies::AddBoids(const int32 CountToAdd)
{
	BoidPool.AddBoids(CountToAdd);
}

void FBoidSpecies::RemoveBoids(const int32 CountToRemove)
{
	BoidPool.RemoveLastUsed(CountToRemove);
}

int32 FBoidSpecies::Num() const
{
	return BoidPool.Num();
}

void FBoidSpecies::UpdateBoidsForceParameters(float NewSeparationForce, float NewAlignmentForce, float NewCohesionForce,
	float NewOtherSpeciesMultiplier)
{
	SpeciesInfo.OverwriteForcesData(NewSeparationForce, NewAlignmentForce, 
	NewCohesionForce, NewOtherSpeciesMultiplier);
}

void FBoidSpecies::UpdateBoidsSpatialAwarenessParameters(float NewDesiredSpeed, float NewPerceptionDistance)
{
	SpeciesInfo.OverwriteSpatialAwarenessData(NewDesiredSpeed, NewPerceptionDistance);
} 

void FBoidSpecies::UpdateBoidCollisionMultiplier(float NewEnvironmentCollisionMultiplier,
	float NewBoundsCollisionMultiplier)
{
	SpeciesInfo.OverwriteCollisionMultiplierData(NewEnvironmentCollisionMultiplier, 
	NewBoundsCollisionMultiplier);
}
