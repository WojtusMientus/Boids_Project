// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoundsData.generated.h"

/**
 * Data asset defining parameters for simulation bounds.
 * Configures bounds center and its extent.
 */
// TODO: Load saved data at simulation start
UCLASS()
class BOIDS_PROJECT_API UBoundsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/** World location of the bounds center. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Center;
	
	/** Extent of the bounds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Extent;
	
};
