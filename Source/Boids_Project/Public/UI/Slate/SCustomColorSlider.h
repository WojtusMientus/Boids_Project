// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSlider.h"





class BOIDS_PROJECT_API SCustomColorSlider : public SCompoundWidget
{

public:
	
	SLATE_BEGIN_ARGS(SCustomColorSlider)
		: _Label(FText::FromString("L")),
		_Value(0.0f),
		_MinValue(0.0f),
		_MaxValue(1.0f),
		_OnValueChanged()
		{}

		SLATE_ARGUMENT(FText, Label)

		SLATE_ATTRIBUTE(float, Value)
		
		SLATE_ARGUMENT(float, MinValue)

		SLATE_ARGUMENT(float, MaxValue)

		SLATE_ARGUMENT(FLinearColor, StartGradientColor)
		
		SLATE_ARGUMENT(FLinearColor, EndGradientColor)

		SLATE_EVENT(FOnFloatValueChanged, OnValueChanged)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);	

protected:

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
		
	FLinearColor StartGradientColor;

	FLinearColor EndGradientColor;
	
	TUniquePtr<FSpinBoxStyle> SpinBoxBrush;
	
	TUniquePtr<FSlateFontInfo> ComboBoxStyle;
	
	TUniquePtr<FSliderStyle> SliderStyle;
	
	TSharedPtr<SSlider> InternalSlider = nullptr;

	TUniquePtr<FSlateBrush> SelectorBrush;
	
	
	static constexpr float Padding = 10.0f;
	static constexpr float LabelSize = 8.0f;
	
	static constexpr float SpinBoxWidth = 40.0f;

	static constexpr float SliderWidth = 115.0f;
	static constexpr float SliderHeight = 18.0f;

	static constexpr int32 NumberOfGradientStops = 10;
};
