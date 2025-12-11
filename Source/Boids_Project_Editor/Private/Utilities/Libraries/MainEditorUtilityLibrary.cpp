
#include "Utilities/Libraries/MainEditorUtilityLibrary.h"
#include "GameplayTagContainer.h"

FString UMainEditorUtilityLibrary::GetGameplayTagLeafName(const FGameplayTag Tag)
{
	return Tag.GetTagLeafName().ToString();
}