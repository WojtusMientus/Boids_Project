
// Boid-specific selection interface for runtime visualization
//
// Future: Could be generalized to ISeectable interface

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableBoid.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class USelectableBoid : public UInterface
{
	GENERATED_BODY()
};

class BOIDS_PROJECT_API ISelectableBoid
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	virtual FBoidSelection GetBoidSelection() const = 0;
	
};
