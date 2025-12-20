
#include "DataAssets/BoidsData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"

void UBoidsData::OverwritePlainData(const FBoidsPlainInfo& NewBoidsData)
{
	Number = NewBoidsData.Number;;
	SeparationForce = NewBoidsData.SeparationForce;
	AlignmentForce = NewBoidsData.AlignmentForce;
	CohesionForce = NewBoidsData.CohesionForce;
	DesiredSpeed = NewBoidsData.DesiredSpeed;
	PerceptionDistance = NewBoidsData.PerceptionDistance;
	PerceptionAngle = NewBoidsData.PerceptionAngle;
	Color = NewBoidsData.Color;
}

FBoidsPlainInfo UBoidsData::GetPlainDataInfo() const
{
	FBoidsPlainInfo PlainDataInfo;
	PlainDataInfo.OverwriteData(this);
	return PlainDataInfo;
}

