// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BoidEditorUtilityWidgetBase.generated.h"

class SCustomColorPicker;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChangedEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCommittedEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCancelledEvent, FLinearColor, NewColor);

/**
 * Base class for Boids Tool. 
 * Its main purpose is to bridge logic between Slate and Blueprints.
 */
UCLASS(Blueprintable)
class UBoidEditorUtilityWidgetBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintAssignable)
	FOnColorChangedEvent OnColorChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnColorCommittedEvent OnColorCommitted;
	
	UPROPERTY(BlueprintAssignable)
	FOnColorCancelledEvent OnColorCancelled;

	/**
	 * Initializes global custom color picker with new starting color.
	 * @param StartingColor Initial color of spawned color picker.
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnColorPicker(const FLinearColor StartingColor);
	
private:
	
	/** Helper functions for broadcasting events. */
	void HandleColorChanged(FLinearColor NewColor);
	void HandleColorCommitted(FLinearColor NewColor);
	void HandleColorCancelled(FLinearColor NewColor);
	
};
