// Copyright WojtusMientus

#include "Libraries/ToolFunctionLibrary.h"
#include "GameplayTagsManager.h"

int32 UToolFunctionLibrary::GetNumChildGameplayTags(const FGameplayTag& Parent)
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	const FGameplayTagContainer& ChildContainer = GameplayTagsManager.RequestGameplayTagChildren(Parent);
	return ChildContainer.Num();
}

