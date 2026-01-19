// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "VisualizerVisibility.generated.h"

/** Helper struct for passing visibility data from UBoidEditorUtilityWidget and its delegate listeners. */
USTRUCT(BlueprintType)
struct FVisualizerVisibility
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsBoundsVisible = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsWallDataVisible = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCollisionDataVisible = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsSimulationAreaVoxelVisible = false;
};