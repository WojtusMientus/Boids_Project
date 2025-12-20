// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameInstanceSubsystemBase.generated.h"


/**
 * Base class for the subsystems.
 * Makes sure that URuntimeDataLoaderSubsystem is initialized first.
 */
UCLASS()
class BOIDS_PROJECT_API UGameInstanceSubsystemBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	//~ Begin UGameInstanceSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End UGameInstanceSubsystem Interface
};
