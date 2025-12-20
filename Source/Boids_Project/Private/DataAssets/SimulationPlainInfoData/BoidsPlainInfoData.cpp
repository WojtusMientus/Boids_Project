
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "DataAssets/BoidsData.h"

FBoidsPlainInfo::FBoidsPlainInfo(const UBoidsData* NewBoidsData)
{
	OverwriteData(NewBoidsData);
}

void FBoidsPlainInfo::OverwriteData(const UBoidsData* NewBoidsData)
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
	DesiredSpeed = NewBoidsData->DesiredSpeed;
	PerceptionDistance = NewBoidsData->PerceptionDistance;
	PerceptionAngle = NewBoidsData->PerceptionAngle;	
}
