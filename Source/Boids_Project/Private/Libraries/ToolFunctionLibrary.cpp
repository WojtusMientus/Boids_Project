// Copyright WojtusMientus

#include "Libraries/ToolFunctionLibrary.h"
#include "GameplayTagsManager.h"
#include "UI/Slate/SCustomColorPicker.h"

int32 UToolFunctionLibrary::GetNumChildGameplayTags(const FGameplayTag& Parent)
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	const FGameplayTagContainer& ChildContainer = GameplayTagsManager.RequestGameplayTagChildren(Parent);
	return ChildContainer.Num();
}
