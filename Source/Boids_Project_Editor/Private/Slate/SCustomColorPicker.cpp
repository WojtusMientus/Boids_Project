
#include "Slate/SCustomColorPicker.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorSpectrum.h"
#include "Slate/SCustomColorSlider.h"


const FVector2D SCustomColorPicker::DEFAULT_WINDOW_SIZE = FVector2D(340, 300);
const FVector2D SCustomColorPicker::ADDITIONAL_WINDOW_SPAWN_PADDING = FVector2D(0, 200);

void SCustomColorPicker::Construct(const FArguments& InArgs)
{
	StartingColor = InArgs._StartingColor;
	ParentWindow = InArgs._ParentWindow;
	OnColorValueChanged = InArgs._OnColorValueChanged;
	OnColorCommitted = InArgs._OnColorCommitted;
	OnColorCancelled = InArgs._OnColorCancelled;
	OnWindowClosed = InArgs._OnWindowClosed;
	
	CurrentColorRGB = StartingColor;
	CurrentColorHSV = StartingColor.LinearRGBToHSV();

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.Padding(PADDING)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(COLOR_SPECTRUM_HEIGHT)
				.WidthOverride(COLOR_SPECTRUM_WIDTH)
				[
					ConstructColorSpectrum()
				]
			]

			+ SVerticalBox::Slot()
			.Padding(0, PADDING, 0, 0)
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
					.Padding(0, PADDING, 0, PADDING)
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
				.Padding(PADDING, 0, 0, 0)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.MinHeight(COLOR_BLOCK_HEIGHT)
					.MaxHeight(COLOR_BLOCK_HEIGHT)
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
					.Padding(FMargin(0.0f, PADDING, 0.0f, 0.0f))
					.MinHeight(BUTTONS_HEIGHT)
					.MaxHeight(BUTTONS_HEIGHT)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Fill)
						.Padding(0, 0, BUTTONS_PADDING, 0)
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
						.Padding(BUTTONS_PADDING, 0, 0, 0)
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
		SliderTooltipText = FText::FromString("Green");
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

FLinearColor SCustomColorPicker::HandleColorSpectrumValue() const
{
	return CurrentColorHSV;
}

void SCustomColorPicker::HandleColorSpectrumValueChanged(FLinearColor InColorValueHSV)
{
	CurrentColorRGB = InColorValueHSV.HSVToLinearRGB().GetClamped(0.0f, FLT_MAX);
	CurrentColorHSV = InColorValueHSV;
	HandleColorValueChanged();
}

FLinearColor SCustomColorPicker::HandleColorBlockValue() const
{
	return CurrentColorRGB;
}

FLinearColor SCustomColorPicker::HandleOldColorBlockValue() const
{
	return StartingColor;
}

void SCustomColorPicker::UpdateCurrentColorHSV()
{
	CurrentColorHSV = CurrentColorRGB.LinearRGBToHSV().GetClamped(0.0f, FLT_MAX);
}

void SCustomColorPicker::HandleColorValueChanged()
{
	OnColorValueChanged.ExecuteIfBound(CurrentColorRGB);
}

FReply SCustomColorPicker::HandleOkButtonClicked()
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

FReply SCustomColorPicker::HandleCancelButtonClicked()
{
	OnColorCancelled.ExecuteIfBound(StartingColor);

	if (ParentWindow.IsValid())
	{
		if (TSharedPtr<SWindow> Window = ParentWindow.Pin())
		{
			Window->RequestDestroyWindow();
		}
	}
	
	return FReply::Handled();
}



static TSharedPtr<SCustomColorPicker> GlobalCustomColorPicker;
static TWeakPtr<SWindow> CustomColorPickerWindow;

void SCustomColorPicker::OpenCustomColorPicker(const FCustomColorPickerArgs& InArgs)
{
	const FVector2D ConstructionLocation = CalculateColorPickerConstructionLocation();
	TSharedRef<SWindow> Window = CreateCustomColorPickerWindow(ConstructionLocation);
	Window->SetOnWindowClosed(FOnWindowClosed::CreateStatic(
	&SCustomColorPicker::HandleWindowClosed));
	
	if (!GlobalCustomColorPicker.IsValid())
	{
		GlobalCustomColorPicker = CreateCustomColorPicker(InArgs, Window);
	}
	else
	{
		SetupNewStartingValues(InArgs, Window);
	}
	
	TryDestroyOldWindow();
	
	Window->SetContent(GlobalCustomColorPicker.ToSharedRef());
	FSlateApplication::Get().AddWindow(Window);
	Window->MoveWindowTo(ConstructionLocation);
	
	CustomColorPickerWindow = Window;
}

void SCustomColorPicker::TryDestroyOldWindow()
{
	if (CustomColorPickerWindow.IsValid())
	{
		if (TSharedPtr<SWindow> ExistingWindow = CustomColorPickerWindow.Pin())
		{
			ExistingWindow->RequestDestroyWindow();
		}
	}
}

FVector2D SCustomColorPicker::CalculateColorPickerConstructionLocation()
{
	const FVector2D CurrentCursorPosition = FSlateApplication::Get().GetCursorPos();
	
	const FSlateRect Anchor(CurrentCursorPosition.X, CurrentCursorPosition.Y,
		CurrentCursorPosition.X, CurrentCursorPosition.Y);

	const FVector2D WindowSize =  DEFAULT_WINDOW_SIZE +  ADDITIONAL_WINDOW_SPAWN_PADDING;
	return FSlateApplication::Get().CalculatePopupWindowPosition(Anchor,
		WindowSize, true, FVector2D::ZeroVector, Orient_Horizontal);
}

TSharedRef<SCustomColorPicker> SCustomColorPicker::CreateCustomColorPicker(const FCustomColorPickerArgs& InArgs,
	 const TSharedRef<SWindow>& InWindow)
{
	return SNew(SCustomColorPicker)
		.StartingColor(InArgs.StartingColor)
		.ParentWindow(InWindow)
		.OnColorValueChanged(InArgs.OnColorValueChanged)
		.OnColorCommitted(InArgs.OnColorCommitted)
		.OnColorCancelled(InArgs.OnColorCancelled)
		.OnWindowClosed(InArgs.OnWindowClosed);
}

TSharedRef<SWindow> SCustomColorPicker::CreateCustomColorPickerWindow(const FVector2D InSpawnLocation)
{
	return SNew(SWindow)
		.Title(FText::FromString("Custom Color Picker"))
		.ScreenPosition(InSpawnLocation)
		.SizingRule(ESizingRule::Autosized)
		.SupportsMinimize(false)
		.SupportsMaximize(false);
}

void SCustomColorPicker::HandleWindowClosed(const TSharedRef<SWindow>& InWindow)
{
	if (GlobalCustomColorPicker.IsValid())
	{
		GlobalCustomColorPicker->OnWindowClosed.ExecuteIfBound();
	}
}

void SCustomColorPicker::SetupNewStartingValues(const FCustomColorPickerArgs& InArgs, const TSharedRef<SWindow>& InWindow)
{
	GlobalCustomColorPicker->SetNewStartingColor(InArgs.StartingColor);
	GlobalCustomColorPicker->SetNewParentWindow(InWindow);
	GlobalCustomColorPicker->OnColorValueChanged = InArgs.OnColorValueChanged;
	GlobalCustomColorPicker->OnColorCommitted = InArgs.OnColorCommitted;
	GlobalCustomColorPicker->OnColorCancelled = InArgs.OnColorCancelled;
	GlobalCustomColorPicker->OnWindowClosed = InArgs.OnWindowClosed;
}

void SCustomColorPicker::SetNewParentWindow(const TSharedRef<SWindow>& InParentWindow)
{
	ParentWindow = InParentWindow;
}

void SCustomColorPicker::SetNewStartingColor(const FLinearColor InColor)
{
	StartingColor = InColor;
	CurrentColorRGB = InColor;
	CurrentColorHSV = InColor.LinearRGBToHSV();
}


