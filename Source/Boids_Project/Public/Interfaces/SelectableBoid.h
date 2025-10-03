// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableBoid.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class USelectableBoid : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for accessing Boid selection information.
 */
class BOIDS_PROJECT_API ISelectableBoid
{
	GENERATED_BODY()

public:
	
	/** Retrieves selection information about this Boid. */
	UFUNCTION(BlueprintCallable)
	virtual FBoidSelectionInfo GetBoidSelection() const = 0;
	
};
