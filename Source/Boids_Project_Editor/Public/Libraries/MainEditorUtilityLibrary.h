// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityLibrary.h"
#include "MainEditorUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BOIDS_PROJECT_EDITOR_API UMainEditorUtilityLibrary : public UEditorUtilityLibrary
{
	GENERATED_BODY()

	/** Retrieves number of child gameplay tags in the hierarchy. */
	UFUNCTION(BlueprintCallable)
	static int32 GetNumberOfChildGameplayTags(const FGameplayTag& Parent);
	
};
