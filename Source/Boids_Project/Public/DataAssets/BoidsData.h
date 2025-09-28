
// Data container for Boid behavior parameters
//
// Future: Load at simulation start, suport external tool integration for customization & saving

#pragma once

#include "CoreMinimal.h"
#include "Boids_Project/Globals.h"
#include "Engine/DataAsset.h"
#include "BoidsData.generated.h"

#define BOID_CATEGORY_SETUP "Boid|Setup"
#define BOID_CATEGORY_FORCES "Boid|Forces"
#define BOID_CATEGORY_PERCEPTION "Boid|Perception"


UCLASS()
class BOIDS_PROJECT_API UBoidsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BOID_CATEGORY_SETUP)
	EBoidType BoidType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BOID_CATEGORY_SETUP)
	int32 BoidNumber = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BOID_CATEGORY_SETUP)
	FColor BoidColor;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_FORCES)
	float SeparationForce = 1;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_FORCES)
	float AlignmentForce = 1;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_FORCES)
	float CohesionForce = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_PERCEPTION)
	float BoidSpeed;
	 
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BOID_CATEGORY_PERCEPTION)
	int32 PerceptionDistance = 100;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 30, ClampMax = 360), Category = BOID_CATEGORY_PERCEPTION)
	int32 PerceptionAngle = 360;
	
};
