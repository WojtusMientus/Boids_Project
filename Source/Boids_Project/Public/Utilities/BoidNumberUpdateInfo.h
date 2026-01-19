// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BoidNumberUpdateInfo.generated.h"


/** 
 *  Helper struct for updating boid at runtime.
 */
USTRUCT(BlueprintType)
struct BOIDS_PROJECT_API FBoidNumberUpdateInfo
{
	
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Type;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Count;
	
	FBoidNumberUpdateInfo() {};
	FBoidNumberUpdateInfo(const FGameplayTag InTag, const int32 InCount): Type(InTag), Count(InCount) {};
};