// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BoidsData.generated.h"

struct FBoidsSpeciesPlainInfo;


/**
 * Data asset defining parameters for a single Boid species.
 * Configures spawning, visual appearance and behavioral forces.
 */
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
	int32 Number = 0;
	
	/** Separation force multiplier. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Forces")
	float SeparationForce = 750.0f;
	
	/** Alignment force multiplier. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Forces")
	float AlignmentForce = 200.0f;
	
	/** Cohesion force multiplier. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Forces")
	float CohesionForce = 750.0f;

	/** Other species force multiplier (Additional multiplier for interspecies interactions 
	 * creating behaviors like avoiding each other or maybe in future chasing different species). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Forces")
	float OtherSpeciesForceMultiplier = 100.0f;
	
	
	/** Desired movement speed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perception")
	float DesiredSpeed = 250.0f;
	 
	/** Maximum perception radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perception")
	float PerceptionDistance = 175.0f;
	
	/** Visual Boid's color. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Color")
	FLinearColor Color = FLinearColor::White;
	
	
	/** Final multiplier applied to environment collision force before retrieving data. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "10000"), Category = "Environment")
	float EnvironmentCollisionMultiplier = 1.0f;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "10000"), Category = "Environment")
	float BoundsCollisionMultiplier = 1.0f;
	
	
	void OverwritePlainData(const FBoidsSpeciesPlainInfo& NewBoidsData);
	FBoidsSpeciesPlainInfo GetPlainDataInfo() const;
};

