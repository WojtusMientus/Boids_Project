#pragma once

#include "CoreMinimal.h"
#include "VisualizerVisibility.generated.h"


USTRUCT(BlueprintType)
struct FVisualizerVisibility
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsBoundsVisible;

	UPROPERTY(BlueprintReadWrite)
	bool bIsWallDataVisible;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCollisionDataVisible;
};