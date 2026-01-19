
#include "DataAssets/BoidsData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"


void UBoidsData::OverwritePlainData(const FBoidsSpeciesPlainInfo& NewBoidsData)
{
	Number = NewBoidsData.Number;
	
	SeparationForce = NewBoidsData.SeparationForce;
	AlignmentForce = NewBoidsData.AlignmentForce;
	CohesionForce = NewBoidsData.CohesionForce;
	OtherSpeciesForceMultiplier = NewBoidsData.OtherSpeciesForceMultiplier;
	
	DesiredSpeed = NewBoidsData.DesiredSpeed;
	PerceptionDistance = NewBoidsData.PerceptionDistance;
	Color = NewBoidsData.Color;
	
	EnvironmentCollisionMultiplier = NewBoidsData.EnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = NewBoidsData.BoundsCollisionMultiplier;
}

FBoidsSpeciesPlainInfo UBoidsData::GetPlainDataInfo() const
{
	return FBoidsSpeciesPlainInfo(this);
}
