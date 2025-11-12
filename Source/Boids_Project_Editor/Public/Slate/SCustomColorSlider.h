// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSlider.h"

/**
 * Custom color slider used in custom color picker.
 * Shows gradient with a handle and its numeric value in the spinbox next to it. 
 */
class BOIDS_PROJECT_EDITOR_API SCustomColorSlider : public SCompoundWidget
{

public:
	
	SLATE_BEGIN_ARGS(SCustomColorSlider)
		: _Label(FText::GetEmpty()),
		_Value(0.0f),
		_MinValue(0.0f),
		_MaxValue(1.0f),
		_StartGradientColor(FLinearColor::White),
		_EndGradientColor(FLinearColor::White),
		_OnValueChanged()
		{}

		/** Content of the label next to the slider. */
		SLATE_ARGUMENT(FText, Label)
		
		/** Current value of a slider. */
		SLATE_ATTRIBUTE(float, Value)

		/** Minimum value of a slider. */
		SLATE_ARGUMENT(float, MinValue)

		/** Maximum value of a slider. */
		SLATE_ARGUMENT(float, MaxValue)

		/** Start color of the slider gradient. */
		SLATE_ARGUMENT(FLinearColor, StartGradientColor)

		/** End color of the slider gradient. */
		SLATE_ARGUMENT(FLinearColor, EndGradientColor)

		/** Delegate called when slider value changes. */
		SLATE_EVENT(FOnFloatValueChanged, OnValueChanged)

	SLATE_END_ARGS()

	/** Construct the widget from arguments. */
	void Construct(const FArguments& InArgs);	

protected:
	
	//~ Begin SWidget Interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	//~ End SWidget Interface
	
private:
		
	/** Initializer functions for Slate brushes and styles. */
	void InitializeSliderStyle();
	void InitializeSliderHandleBrush();
	void InitializeBackgroundBoxBrush();
	void InitializeComboBoxFontStyle();
	
	
	/** Colors used in the gradient. */
	FLinearColor StartGradientColor;
	FLinearColor EndGradientColor;
	
	
	/** Style used to draw internal slider. */
	FSliderStyle SliderStyle;
	
	/** Font info for the spinbox. */
	FSlateFontInfo SpinBoxFontInfo;
	
	/** Style used to draw spinbox and slider outline. */
	FSpinBoxStyle BackgroundBoxStyle;
	
	/** Brush used to draw handle of the custom slider. */
	FSlateBrush SliderHandleBrush;
	
	/** Internal slider widget */
	TSharedPtr<SSlider> InternalSlider;
	
	/** Constants */
	static constexpr float PADDING = 10.0f;
	static constexpr float LABEL_SIZE = 8.0f;
	
	static constexpr float SPIN_BOX_WIDTH = 40.0f;

	static constexpr float SLIDER_WIDTH = 115.0f;
	static constexpr float SLIDER_HEIGHT = 18.0f;
	static constexpr int32 SLIDER_OUTLINE_WIDTH = 1;
	
	static constexpr int32 GRADIENT_STOPS_COUNT = 10;
};
