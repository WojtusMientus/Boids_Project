// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoundsData.generated.h"

/**
 * 
 */
UCLASS()
class BOIDS_PROJECT_API UBoundsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector BoundsCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector BoundsExtent;
	
};
