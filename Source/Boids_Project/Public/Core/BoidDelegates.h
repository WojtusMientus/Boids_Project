#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnColorUpdateEvent, FGameplayTag /* Tag */, FLinearColor /* NewColor */);

namespace BoidsDelegates
{
	inline FOnColorUpdateEvent& OnColorUpdate()
	{
		static FOnColorUpdateEvent InstancedEvent;
		return InstancedEvent;
	}
}
