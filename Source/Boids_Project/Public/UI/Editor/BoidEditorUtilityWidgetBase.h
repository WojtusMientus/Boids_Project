// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BoidEditorUtilityWidgetBase.generated.h"

class SCustomColorPicker;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChangedEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCommittedEvent, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCancelledEvent, FLinearColor, NewColor);


UCLASS(Blueprintable)
class UBoidEditorUtilityWidgetBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(Blueprintassignable)
	FOnColorChangedEvent OnColorChanged;

	UPROPERTY(BlueprintAssignable)
	FOnColorCommittedEvent OnColorCommitted;

	UPROPERTY(BlueprintAssignable)
	FOnColorCancelledEvent OnColorCancelled;
	
	UFUNCTION(BlueprintCallable)
	void CreateAndPushCustomColorPickerWindow(const FLinearColor StartingColor);

	UFUNCTION(BlueprintCallable)
	void TestFun();
	
private:

	FVector2D CalculateColorPickerConstructionLocation() const;

	TSharedRef<SCustomColorPicker> CreateCustomColorPicker(const FLinearColor StartingColor,
		const TSharedRef<SWindow>& ParentWindow) const;
	TSharedRef<SWindow> CreateCustomColorPickerWindow(const FVector2D SpawnLocation) const;
	
	void HandleColorChanged(FLinearColor NewColor) const;
	void HandleColorCommitted(FLinearColor NewColor) const;
	void HandleColorCancelled(FLinearColor NewColor) const;

	const FVector2D AdditionalColorPickerWindowSpawnPadding = FVector2D(0, 200);
	
	TWeakPtr<SWindow> ColorPickerWindow;

	TSharedPtr<SCustomColorPicker> TestCustomColorPicker;
};
