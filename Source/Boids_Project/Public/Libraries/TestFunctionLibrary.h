// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestFunctionLibrary.generated.h"


UCLASS()
class BOIDS_PROJECT_API UTestFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void ShowFirstEveryWidget();

	UFUNCTION(BlueprintCallable)
	static void ShowColorPicker();

	UFUNCTION(BlueprintCallable)
	static void ShowMyColorSlider();
	
	UFUNCTION(BlueprintCallable)
	static void ShowMyColorPicker();
	
	
	void OldFunction();

	
	static FReply OnButtonClicked();

	
	// TEST COLOR PICKER //

	static FLinearColor ColorRGB;
	static FLinearColor ColorHSV;

	static float SliderValue;
	
	static FLinearColor GetCurrentColor()
	{
		return ColorHSV;
	}

	static float GetCurrentFloat()
	{
		return SliderValue;
	}

	static EVisibility HandleColorPickerVisibility()
	{
		return EVisibility::Visible;
	}

	static void HandleOnValueChanged(FLinearColor InColor)
	{
		// if (FMath::IsNearlyZero(InColor.B))
		// {
		// 	InColor.B = 1.0f;
		// }
		// if (FMath::IsNearlyZero(InColor.A))
		// {
		// 	InColor.A = 1.0f;
		// }

		// SetNewTargetColorFromHSV(InColor);
	}

	void SetNewTargetColorFromHSV(FLinearColor InColor)
	{
		ColorRGB = InColor.HSVToLinearRGB().GetClamped(0.0f, FLT_MAX);
		ColorHSV = InColor;
		UE_LOG(LogTemp, Warning, TEXT("Color: %s"), *ColorRGB.ToString());
	}

	static void HandleOnTestSliderValueChange(float NewValue)
	{
		SliderValue = NewValue;
	}
	
};
