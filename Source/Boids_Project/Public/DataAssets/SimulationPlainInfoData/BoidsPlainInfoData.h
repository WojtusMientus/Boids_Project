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
struct BOIDS_PROJECT_API FBoidsPlainInfo
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
	float DesiredSpeed = 100.0f;
	 
	UPROPERTY(BlueprintReadWrite)
	int32 PerceptionDistance = 100;

	UPROPERTY(BlueprintReadWrite)
	int32 PerceptionAngle = 360;
	
	
	void OverwriteData(const UBoidsData* NewBoidsData);
};