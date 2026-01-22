// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Utilities/BoidNumberUpdateInfo.h"




DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBoidColorUpdateEvent, const FGameplayTag Tag, const FLinearColor NewColor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoidNumberUpdateEvent, const FBoidNumberUpdateInfo& BoidNumberUpdateInfo);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoidNumberUpdateFinishEvent, const TArray<FBoidNumberUpdateInfo>& UpdatedBoidNumber);


DECLARE_MULTICAST_DELEGATE_FiveParams(FOnBoidForceParametersChangeEvent, const FGameplayTag Tag, 
	float NewSeparationForce, float NewAlignmentForce, float NewCohesionForce, float NewOtherSpeciesMultiplier);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnBoidSpatialAwarenessParametersChangeEvent, const FGameplayTag Tag,
	float NewDesiredSpeed, float NewPerceptionDistance);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnBoidCollisionMultiplierUpdateEvent, const FGameplayTag Tag, 
	float NewEnvironmentCollisionMultiplier, float NewBoundsCollisionMultiplier);


namespace BoidsDelegates
{
	extern BOIDS_PROJECT_API FOnBoidColorUpdateEvent OnBoidColorUpdate;
	extern BOIDS_PROJECT_API FOnBoidNumberUpdateEvent OnBoidNumberUpdate;
	extern BOIDS_PROJECT_API FOnBoidNumberUpdateFinishEvent OnBoidNumberUpdateFinish;
	
	extern BOIDS_PROJECT_API FOnBoidForceParametersChangeEvent OnBoidForceParametersChange;
	extern BOIDS_PROJECT_API FOnBoidSpatialAwarenessParametersChangeEvent OnBoidSpatialAwarenessParametersChange;
	extern BOIDS_PROJECT_API FOnBoidCollisionMultiplierUpdateEvent OnBoidCollisionMultiplierChange;
}
