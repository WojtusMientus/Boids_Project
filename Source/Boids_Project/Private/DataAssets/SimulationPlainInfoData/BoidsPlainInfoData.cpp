
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "DataAssets/BoidsData.h"

FBoidsSpeciesPlainInfo::FBoidsSpeciesPlainInfo(const UBoidsData* NewBoidsData)
{
	OverwriteData(NewBoidsData);
}

void FBoidsSpeciesPlainInfo::OverwriteData(const UBoidsData* NewBoidsData)
{
	if (!NewBoidsData)
	{
		return;
	}
	
	Type = NewBoidsData->Type;
	Number = NewBoidsData->Number;
	Color = NewBoidsData->Color;
	
	SeparationForce = NewBoidsData->SeparationForce;
	AlignmentForce = NewBoidsData->AlignmentForce;
	CohesionForce = NewBoidsData->CohesionForce;
	OtherSpeciesForceMultiplier = NewBoidsData->OtherSpeciesForceMultiplier;
	
	DesiredSpeed = NewBoidsData->DesiredSpeed;
	PerceptionDistance = NewBoidsData->PerceptionDistance;
	
	EnvironmentCollisionMultiplier = NewBoidsData->EnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = NewBoidsData->BoundsCollisionMultiplier;
}

void FBoidsSpeciesPlainInfo::OverwriteForcesData(float NewSeparationForce, float NewAlignmentForce, float NewCohesionForce,
	float NewOtherSpeciesMultiplier)
{
	SeparationForce = NewSeparationForce;
	AlignmentForce = NewAlignmentForce;
	CohesionForce = NewCohesionForce;
	OtherSpeciesForceMultiplier = NewOtherSpeciesMultiplier;
}

void FBoidsSpeciesPlainInfo::OverwriteSpatialAwarenessData(float NewDesiredSpeed, float NewPerceptionDistance)
{
	DesiredSpeed = NewDesiredSpeed;
	PerceptionDistance = NewPerceptionDistance;
	PerceptionDistanceSquared = PerceptionDistance * PerceptionDistance;
}

void FBoidsSpeciesPlainInfo::OverwriteCollisionMultiplierData(float NewEnvironmentCollisionMultiplier,
	float NewBoundsCollisionMultiplier)
{
	EnvironmentCollisionMultiplier = NewEnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = NewBoundsCollisionMultiplier;
}
