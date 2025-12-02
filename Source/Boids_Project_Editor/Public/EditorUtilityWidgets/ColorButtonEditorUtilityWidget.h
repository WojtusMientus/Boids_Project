// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ColorButtonEditorUtilityWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChangedEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCommittedEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCancelledEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnColorPickerWindowClosedEvent);


/**
 * Base class for blueprint color button widget.
 * Able to create color picker and bridge logic between Slate and Blueprints. 
 */
UCLASS(Blueprintable)
class BOIDS_PROJECT_EDITOR_API UColorButtonEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
protected:
		
	UPROPERTY(BlueprintAssignable)
	FOnColorChangedEvent OnColorChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnColorCommittedEvent OnColorCommitted;
	
	UPROPERTY(BlueprintAssignable)
	FOnColorCancelledEvent OnColorCancelled;
	
	UPROPERTY(BlueprintAssignable)
	FOnColorPickerWindowClosedEvent OnColorPickerWindowClosed;
	
	//~ Begin UEditorUtilityWidget Interface 
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UEditorUtilityWidget Interface 
	
	/**
	 * Initializes global custom color picker with new starting color.
	 * @param StartingColor Initial color of spawned color picker.
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnColorPicker(const FLinearColor StartingColor = FLinearColor::White);

	
private:
	
	/** Helper functions for broadcasting events. */
	void HandleColorChanged(FLinearColor NewColor);
	void HandleColorCommitted(FLinearColor NewColor);
	void HandleColorCancelled(FLinearColor NewColor);
	void HandleColorPickerWindowClosed();
	
};
