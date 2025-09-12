// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boids_Project/Globals.h"
#include "Engine/DataAsset.h"
#include "BoidsData.generated.h"


UCLASS()
class BOIDS_PROJECT_API UBoidsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EBoidType BoidType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BoidNumber = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor BoidColor;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float SeparationForce = 1;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float AlignmentForce = 1;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float CohesionForce = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float BoidSpeed;
	 
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 PerceptionDistance = 100;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 30, ClampMax = 360))
	int32 PerceptionAngle = 360;
	
};
