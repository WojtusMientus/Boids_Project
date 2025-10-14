// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BoidsData.generated.h"


/**
 * Data asset defining parameters for a single Boid species.
 * Configures spawning, visual appearance and behavioral forces.
 */
// TODO: Load saved data at simulation start
UCLASS()
class BOIDS_PROJECT_API UBoidsData : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Species type of the Boid. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	FGameplayTag Type;
	
	/** Number of Boids of this species to spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	int32 Number = 100;

	/** Visual Boid's color. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	FColor Color;
	
	/** Separation force multiplier. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Forces")
	float SeparationForce = 100.0f;
	
	/** Alignment force multiplier. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Forces")
	float AlignmentForce = 100.0f;
	
	/** Cohesion force multiplier. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Forces")
	float CohesionForce = 100.0f;

	/** Desired movement speed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perception")
	float DesiredSpeed;
	 
	/** Maximum perception radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perception")
	int32 PerceptionDistance = 100;

	/** Maximum perception angle. */
	// TODO: Implement angular perception
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 30, ClampMax = 360), Category = "Perception")
	int32 PerceptionAngle = 360;
	
};
