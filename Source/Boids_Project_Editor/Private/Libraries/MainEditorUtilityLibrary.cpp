
#include "Libraries/MainEditorUtilityLibrary.h"
#include "GameplayTagsManager.h"


int32 UMainEditorUtilityLibrary::GetNumberOfChildGameplayTags(const FGameplayTag& Parent)
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	const FGameplayTagContainer& ChildContainer = GameplayTagsManager.RequestGameplayTagChildren(Parent);
	return ChildContainer.Num();
}
