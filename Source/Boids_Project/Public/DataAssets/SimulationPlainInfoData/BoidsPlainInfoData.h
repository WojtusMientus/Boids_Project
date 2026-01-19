// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BoidsPlainInfoData.generated.h"

class UBoidsData;


/**
 * Helper struct for passing data between tool and EditorBoidDataManager for saving and simplicity purposes.
 */
USTRUCT(BlueprintType)
struct BOIDS_PROJECT_API FBoidsSpeciesPlainInfo
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Type;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Number = 0;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor Color;
	 
	
	UPROPERTY(BlueprintReadWrite)
	float SeparationForce = 100.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float AlignmentForce = 100.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float CohesionForce = 100.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float OtherSpeciesForceMultiplier = 0.0f;
	

	UPROPERTY(BlueprintReadWrite)
	float DesiredSpeed = 100.0f;
	 
	UPROPERTY(BlueprintReadWrite)
	float PerceptionDistance = 100;
	
	
	UPROPERTY(BlueprintReadWrite)
	float EnvironmentCollisionMultiplier = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float BoundsCollisionMultiplier = 1.0f;
	
	
	float PerceptionDistanceSquared = 0.0f;
	
	FIntVector PerceptionDistanceInVoxelCellCount = FIntVector();
	
	
	
	FBoidsSpeciesPlainInfo(){};
	FBoidsSpeciesPlainInfo(const UBoidsData* NewBoidsData);
	
	
	void OverwriteData(const UBoidsData* NewBoidsData);
	
	void OverwriteForcesData(	float NewSeparationForce, float NewAlignmentForce, float NewCohesionForce,
		float NewOtherSpeciesMultiplier);
	void OverwriteSpatialAwarenessData(float NewDesiredSpeed, float NewPerceptionDistance);
	void OverwriteCollisionMultiplierData(float NewEnvironmentCollisionMultiplier, float NewBoundsCollisionMultiplier);	
};