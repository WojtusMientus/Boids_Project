// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityLibrary.h"
#include "ToolFunctionLibrary.generated.h"


UCLASS()
class BOIDS_PROJECT_API UToolFunctionLibrary : public UEditorUtilityLibrary
{
	GENERATED_BODY()

	/** Retrieves number of child gameplay tags in the hierarchy. */
	UFUNCTION(BlueprintCallable)
	static int32 GetNumChildGameplayTags(const FGameplayTag& Parent);
	
};
