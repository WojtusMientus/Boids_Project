// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BoidGameInstance.generated.h"


/**
 * Custom game instance for loading simulation data, after all the subsystems are initialized.
 */
UCLASS()
class BOIDS_PROJECT_API UBoidGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	//~ Begin UGameInstance Interface
	virtual void Init() override;
	//~ End UGameInstance Interface
};
