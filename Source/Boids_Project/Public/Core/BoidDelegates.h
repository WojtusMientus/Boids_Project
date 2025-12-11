
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
	inline FOnBoidColorUpdateEvent& OnBoidColorUpdate()
	{
		static FOnBoidColorUpdateEvent InstancedEvent;
		return InstancedEvent;
	}
	
	inline FOnCollisionMultiplierUpdateEvent& OnCollisionMultiplierUpdate()
	{
		static FOnCollisionMultiplierUpdateEvent InstancedEvent;
		return InstancedEvent;
	}
	
	inline FOnBoidNumberUpdateEvent& OnBoidNumberUpdate()
	{
		static FOnBoidNumberUpdateEvent InstancedEvent;
		return InstancedEvent;
	}
	
	inline FOnBoidNumberUpdateFinishEvent& OnBoidNumberUpdateFinish()
	{
		static FOnBoidNumberUpdateFinishEvent InstancedEvent;
		return InstancedEvent;
	}
	
	inline FOnBoidParameterChangeEvent& OnBoidParameterChange()
	{
		static FOnBoidParameterChangeEvent InstancedEvent;
		return InstancedEvent;
	}
}
