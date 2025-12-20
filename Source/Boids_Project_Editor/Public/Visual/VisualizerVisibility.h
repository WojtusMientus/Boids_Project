#pragma once

#include "CoreMinimal.h"
#include "VisualizerVisibility.generated.h"


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
};