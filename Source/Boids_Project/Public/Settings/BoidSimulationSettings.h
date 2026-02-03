// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "BoidSimulationSettings.generated.h"

class AVisualBoid;


/**
 * Settings for initializing UVisualBoidManagerSubsystem.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Boid Simulation Settings"))
class BOIDS_PROJECT_API UBoidSimulationSettings: public UDeveloperSettings
{
	
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UStaticMesh> BoidMesh = nullptr;
};
