// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class ECustomColorSliderChannel
{
	None,
	Red,
	Green,
	Blue
};


class BOIDS_PROJECT_API SCustomColorPicker : public SCompoundWidget
{
	
public:
	
	SLATE_BEGIN_ARGS(SCustomColorPicker) {}

		SLATE_ARGUMENT(FLinearColor, OldColor);

		SLATE_ARGUMENT(TSharedPtr<SWindow>, ParentWindow)
		
		SLATE_EVENT(FOnLinearColorValueChanged, OnColorValueChanged)
		
		SLATE_EVENT(FOnLinearColorValueChanged, OnColorCommitted)
		
		SLATE_EVENT(FOnLinearColorValueChanged, OnColorCancelled)
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetParentWindow(const TSharedRef<SWindow>& InParentWindow);
	void SetStartingColor(const FLinearColor InColor);
	
	static FVector2D DEFAULT_WINDOW_SIZE;
	
private:

	TSharedRef<SWidget> ConstructColorSlider(ECustomColorSliderChannel Channel);
	TSharedRef<SWidget> ConstructColorSpectrum();
	
	float HandleColorSliderValue(ECustomColorSliderChannel Channel) const;
	void HandleColorSliderValueChanged(float NewValue, ECustomColorSliderChannel Channel);
	
	void HandleColorSpectrumValueChanged(FLinearColor NewColorValue);
	FLinearColor HandleColorSpectrumValue() const;

	FLinearColor HandleColorBlockValue() const;
	FLinearColor HandleOldColorBlockValue() const;
	
	void UpdateCurrentColorHSV();

	void HandleColorValueChanged() const;
	FReply HandleOkButtonClicked() const;
	FReply HandleCancelButtonClicked() const;

	TWeakPtr<SWindow> ParentWindow;

	FOnLinearColorValueChanged OnColorValueChanged;
	FOnLinearColorValueChanged OnColorCommitted;
	FOnLinearColorValueChanged OnColorCancelled;
	
	FLinearColor OldColor = FLinearColor::White;
	FLinearColor CurrentColorRGB;
	FLinearColor CurrentColorHSV;
	
	static constexpr float ColorSpectrumWidth = 320.0f;
	static constexpr float ColorSpectrumHeight = 200.0f;

	static constexpr float ColorBlockHeight = 50.0f;

	static constexpr float ButtonsHeight = 20.0f;
	static constexpr float ButtonsPadding = 3.0f;
	
	static constexpr float Padding = 10.0f;
};
