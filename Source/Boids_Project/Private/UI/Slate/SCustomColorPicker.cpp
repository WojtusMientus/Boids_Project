// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Slate/SCustomColorPicker.h"

#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorSpectrum.h"
#include "UI/Slate/SCustomColorSlider.h"

FVector2D SCustomColorPicker::DEFAULT_WINDOW_SIZE = FVector2D(340, 300);

void SCustomColorPicker::Construct(const FArguments& InArgs)
{
	OldColor = InArgs._OldColor;
	ParentWindow = InArgs._ParentWindow;
	OnColorValueChanged = InArgs._OnColorValueChanged;
	OnColorCommitted = InArgs._OnColorCommitted;
	OnColorCancelled = InArgs._OnColorCancelled;
	
	CurrentColorRGB = OldColor;
	CurrentColorHSV = OldColor.LinearRGBToHSV();

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.Padding(Padding)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(ColorSpectrumHeight)
				.WidthOverride(ColorSpectrumWidth)
				[
					ConstructColorSpectrum()
				]
			]

			+ SVerticalBox::Slot()
			.Padding(0, Padding, 0, 0)
			.VAlign(VAlign_Fill)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						ConstructColorSlider(ECustomColorSliderChannel::Red)
					]

					+ SVerticalBox::Slot()
					.Padding(0, Padding, 0, Padding)
					.AutoHeight()
					[
						ConstructColorSlider(ECustomColorSliderChannel::Green)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						ConstructColorSlider(ECustomColorSliderChannel::Blue)
					]
				]

				+ SHorizontalBox::Slot()
				.Padding(Padding, 0, 0, 0)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.MinHeight(ColorBlockHeight)
					.MaxHeight(ColorBlockHeight)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						[
							SNew(SColorBlock)
							.CornerRadius(FVector4(2, 0, 0, 2))
							.Color(this, &SCustomColorPicker::HandleColorBlockValue)
							.ToolTipText(FText::FromString("Preview of the currently selected color"))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SColorBlock)
							.CornerRadius(FVector4(0, 2, 2, 0))
							.Color(this, &SCustomColorPicker::HandleOldColorBlockValue)
							.ToolTipText(FText::FromString("Preview of the previously selected color"))
						]
					]

					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.0f, Padding, 0.0f, 0.0f))
					.MinHeight(ButtonsHeight)
					.MaxHeight(ButtonsHeight)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						.Padding(0, 0, ButtonsPadding, 0)
						[
							SNew(SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.ContentPadding(FMargin())
							.Text(FText::FromString("Ok"))
							.OnClicked(this,  &SCustomColorPicker::HandleOkButtonClicked)
						]

						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						.Padding(ButtonsPadding, 0, 0, 0)
						[
							SNew(SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.ContentPadding(FMargin(-10, 0))
							.Text(FText::FromString("Cancel"))
							.OnClicked(this,  &SCustomColorPicker::HandleCancelButtonClicked)
						]
					]
				]
			]
		]
	];
}

void SCustomColorPicker::SetParentWindow(const TSharedRef<SWindow>& InParentWindow)
{
	ParentWindow = InParentWindow;
}

void SCustomColorPicker::SetStartingColor(const FLinearColor InColor)
{
	OldColor = InColor;
	CurrentColorRGB = InColor;
	CurrentColorHSV = InColor.LinearRGBToHSV();
}

TSharedRef<SWidget> SCustomColorPicker::ConstructColorSlider(ECustomColorSliderChannel Channel)
{
	FText SliderLabel;
	FText SliderTooltipText;
	FLinearColor SliderEndGradientColor;
	ECustomColorSliderChannel ChannelToUse;

	switch (Channel)
	{
	case ECustomColorSliderChannel::Red:
		SliderLabel = FText::FromString("R");
		SliderTooltipText = FText::FromString("Red");
		SliderEndGradientColor = FLinearColor::Red;
		ChannelToUse = ECustomColorSliderChannel::Red;
		break;
		
	case ECustomColorSliderChannel::Green:
		SliderLabel = FText::FromString("G");
		SliderTooltipText = FText::FromString("Greed");
		SliderEndGradientColor = FLinearColor::Green;
		ChannelToUse = ECustomColorSliderChannel::Green;
		break;
		
	case ECustomColorSliderChannel::Blue:
		SliderLabel = FText::FromString("B");
		SliderTooltipText = FText::FromString("Blue");
		SliderEndGradientColor = FLinearColor::Blue;
		ChannelToUse = ECustomColorSliderChannel::Blue;
		break;
		
	default:
		SliderLabel = FText::FromString("W");
		SliderTooltipText = FText::FromString("White");
		SliderEndGradientColor = FLinearColor::White;
		ChannelToUse = ECustomColorSliderChannel::None;
	}

	return SNew(SCustomColorSlider)
		.Label(SliderLabel)
		.ToolTipText(SliderTooltipText)
		.StartGradientColor(FLinearColor::Black)
		.EndGradientColor(SliderEndGradientColor)
		.Value(this, &SCustomColorPicker::HandleColorSliderValue, ChannelToUse)
		.OnValueChanged(this, &SCustomColorPicker::HandleColorSliderValueChanged,
		                ChannelToUse);
}

TSharedRef<SWidget> SCustomColorPicker::ConstructColorSpectrum()
{
	return SNew(SColorSpectrum)
		.SelectedColor(this, &SCustomColorPicker::HandleColorSpectrumValue)
		.OnValueChanged(this, &SCustomColorPicker::HandleColorSpectrumValueChanged);
}

float SCustomColorPicker::HandleColorSliderValue(ECustomColorSliderChannel Channel) const
{
	switch (Channel)
	{
	case ECustomColorSliderChannel::Red:
		return CurrentColorRGB.R;
	case ECustomColorSliderChannel::Green:
		return CurrentColorRGB.G;
	case ECustomColorSliderChannel::Blue:
		return CurrentColorRGB.B;
	default:
		return 0.0f;
	}
}

void SCustomColorPicker::HandleColorSliderValueChanged(float NewValue, ECustomColorSliderChannel Channel)
{
	switch (Channel)
	{
	case ECustomColorSliderChannel::Red:
		CurrentColorRGB.R = NewValue;
		break;
	case ECustomColorSliderChannel::Green:
		CurrentColorRGB.G = NewValue;
		break;
	case ECustomColorSliderChannel::Blue:
		CurrentColorRGB.B = NewValue;
		break;
	default:
		break;
	}

	UpdateCurrentColorHSV();
	HandleColorValueChanged();
}

void SCustomColorPicker::HandleColorSpectrumValueChanged(FLinearColor NewColorValue)
{
	CurrentColorRGB = NewColorValue.HSVToLinearRGB().GetClamped(0.0f, FLT_MAX);
	CurrentColorHSV = NewColorValue;
	HandleColorValueChanged();
}

FLinearColor SCustomColorPicker::HandleColorSpectrumValue() const
{
	return CurrentColorHSV;
}

FLinearColor SCustomColorPicker::HandleColorBlockValue() const
{
	return CurrentColorRGB;
}

FLinearColor SCustomColorPicker::HandleOldColorBlockValue() const
{
	return OldColor;
}

void SCustomColorPicker::UpdateCurrentColorHSV()
{
	CurrentColorHSV = CurrentColorRGB.LinearRGBToHSV().GetClamped(0.0f, FLT_MAX);
}

void SCustomColorPicker::HandleColorValueChanged() const
{
	OnColorValueChanged.ExecuteIfBound(CurrentColorRGB);
}

FReply SCustomColorPicker::HandleOkButtonClicked() const
{
	OnColorCommitted.ExecuteIfBound(CurrentColorRGB);

	if (ParentWindow.IsValid())
	{
		if (TSharedPtr<SWindow> Window = ParentWindow.Pin())
		{
			Window->RequestDestroyWindow();
		}
	}
	
	return FReply::Handled();
}

FReply SCustomColorPicker::HandleCancelButtonClicked() const
{
	OnColorCancelled.ExecuteIfBound(OldColor);

	if (ParentWindow.IsValid())
	{
		if (TSharedPtr<SWindow> Window = ParentWindow.Pin())
		{
			Window->RequestDestroyWindow();
		}
	}
	
	return FReply::Handled();
}
