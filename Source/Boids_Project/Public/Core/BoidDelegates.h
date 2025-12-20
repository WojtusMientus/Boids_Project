// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBoidColorUpdateEvent, const FGameplayTag Tag, const FLinearColor NewColor);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCollisionMultiplierUpdateEvent, float EnvironmentCollisionMultiplier,
	 float BoundsCollisionMultiplier);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBoidNumberUpdateEvent, const FGameplayTag Tag, int32 CountToUpdate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBoidNumberUpdateFinishEvent, const FGameplayTag Tag, int32 NewBoidCount);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoidParameterChangeEvent, const FBoidsPlainInfo& BoidInfo);


namespace BoidsDelegates
{
	extern BOIDS_PROJECT_API FOnBoidColorUpdateEvent OnBoidColorUpdate;
	extern BOIDS_PROJECT_API FOnCollisionMultiplierUpdateEvent OnCollisionMultiplierUpdate;
	extern BOIDS_PROJECT_API FOnBoidNumberUpdateEvent OnBoidNumberUpdate;
	extern BOIDS_PROJECT_API FOnBoidNumberUpdateFinishEvent OnBoidNumberUpdateFinish;
	extern BOIDS_PROJECT_API FOnBoidParameterChangeEvent OnBoidParameterChange;
}
