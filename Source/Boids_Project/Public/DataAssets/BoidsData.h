// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Boids_Project/Globals.h"
#include "Engine/DataAsset.h"
#include "BoidsData.generated.h"

/** Categories for editor organization. */
#define BOID_CATEGORY_SETUP "Boid|Setup"
#define BOID_CATEGORY_FORCES "Boid|Forces"
#define BOID_CATEGORY_PERCEPTION "Boid|Perception"

/**
 * Data asset defining parameters for a single Boid species.
 * Configures spawning, visual appearance and behavioral forces.
 */
// TODO: Load saved data at simulation start
UCLASS()
class BOIDS_PROJECT_API UBoidsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Species type of the Boid. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BOID_CATEGORY_SETUP)
	EBoidType BoidType;
	
	/** Number of Boids of this species to spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BOID_CATEGORY_SETUP)
	int32 BoidNumber = 100;

	/** Visual Boid's color. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BOID_CATEGORY_SETUP)
	FColor BoidColor;
	
	/** Separation force multiplier. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_FORCES)
	float SeparationForce = 1;
	
	/** Alignment force multiplier. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_FORCES)
	float AlignmentForce = 1;
	
	/** Cohesion force multiplier. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_FORCES)
	float CohesionForce = 1;

	/** Maximum movement speed. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_PERCEPTION)
	float BoidSpeed;
	 
	/** Maximum perception radius. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_PERCEPTION)
	int32 PerceptionDistance = 100;

	/** Maximum perception angle. */
	// TODO: Implement angular perception
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 30, ClampMax = 360), Category = BOID_CATEGORY_PERCEPTION)
	int32 PerceptionAngle = 360;
	
};
