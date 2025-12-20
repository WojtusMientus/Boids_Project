// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityLibrary.h"
#include "MainEditorUtilityLibrary.generated.h"


/**
 * Main editor utility library. Used for testing purposes.
 */
UCLASS()
class BOIDS_PROJECT_EDITOR_API UMainEditorUtilityLibrary : public UEditorUtilityLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetGameplayTagLeafName(FGameplayTag Tag);
};
