// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Boids_Project/Globals.h"
#include "SelectableBoid.generated.h"

// This class does not need to be modified.
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
	virtual FBoidSelection OnBoidSelection();
	
};
