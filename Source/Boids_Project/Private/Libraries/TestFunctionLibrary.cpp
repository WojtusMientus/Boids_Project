// Copyright WojtusMientus

#include "Libraries/TestFunctionLibrary.h"

#include "Widgets/SWindow.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Colors/SColorSpectrum.h"
#include "Widgets/Input/SSlider.h"
#include "UI/Slate/SCustomColorPicker.h"
#include "UI/Slate/SCustomColorSlider.h"



FLinearColor UTestFunctionLibrary::ColorRGB = FLinearColor::White;
FLinearColor UTestFunctionLibrary::ColorHSV = ColorRGB.LinearRGBToHSV();
float UTestFunctionLibrary::SliderValue = 0.5f;

void UTestFunctionLibrary::ShowFirstEveryWidget()
{
	TSharedRef<STextBlock> MyTextBlock = SNew(STextBlock)
		.Text(FText::FromString("Hello World"))
		.ColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	
	
	TSharedRef<SVerticalBox> MySlate = SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	[
		SNew(SImage).ColorAndOpacity(FSlateColor())
	]
	+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
	[
		MyTextBlock
	]
	+ SVerticalBox::Slot()
	[
		SNew(SButton).OnClicked_Lambda([MyTextBlock]()->FReply
		{
			MyTextBlock->SetText(FText::FromString("Button Clicked :>"));
			return FReply::Handled();
		})
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock).Text(FText::FromString("Hello World"))
		]
	];

	TSharedRef<SWindow> Window = SNew(SWindow).ClientSize(FDeprecateSlateVector2D(200,200));
	Window->SetContent(MySlate);
	FSlateApplication::Get().AddWindow(Window);
}

float InverseLerp(float MinValue, float MaxValue, float Alpha);


void UTestFunctionLibrary::ShowColorPicker()
{
	// FMargin PaddingMargin(20);
	//
	// TSharedRef<SVerticalBox> ColorPickerBox = SNew(SVerticalBox)
	// 	+ SVerticalBox::Slot()
	// 	.Padding(PaddingMargin)
	// 	.VAlign(VAlign_Center)
	// 	.HAlign(HAlign_Center)
	// 	[
	// 		SNew(SColorPicker)
	// 	];
	//
	//
	// TSharedRef<SWindow> Window = SNew(SWindow)
	// 	.Title(FText::FromString("Color Picker"))
	// 	.SizingRule(ESizingRule::Autosized);
	// Window->SetContent(ColorPickerBox);
	// FSlateApplication::Get().AddWindow(Window);

	OpenColorPicker(FColorPickerArgs());
	
}

void UTestFunctionLibrary::ShowMyColorSlider()
{
	TSharedRef<SVerticalBox> ColorSliderBox = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.MinHeight(100)
		[
			SNew(SCustomColorSlider)
			.Value_Static(&UTestFunctionLibrary::GetCurrentFloat)
			.StartGradientColor(FLinearColor::Black)
			.EndGradientColor(FLinearColor::Red)
			.OnValueChanged_Static(&UTestFunctionLibrary::HandleOnTestSliderValueChange)
		];
	
	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString("Color Slider Test"))
		.SizingRule(ESizingRule::Autosized)
		.SupportsMinimize(false)
		.SupportsMaximize(false);
	Window->SetContent(ColorSliderBox);
	FSlateApplication::Get().AddWindow(Window);
}

void UTestFunctionLibrary::ShowMyColorPicker()
{

	TSharedRef<SVerticalBox> ColorSliderBox = SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SNew(SCustomColorPicker)
		.OldColor(FLinearColor::White)
	];
	
	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString("Custom Color Picker"))
		.SizingRule(ESizingRule::Autosized)
		.SupportsMinimize(false)
		.SupportsMaximize(false);
	Window->SetContent(ColorSliderBox);
	FSlateApplication::Get().AddWindow(Window);
}


void UTestFunctionLibrary::OldFunction()
{

	TSharedRef<SImage> ColorImage = SNew(SImage).DesiredSizeOverride(TOptional<FVector2D>(FVector2D(50,50)));

	// // TSharedRef<SColorPicker> ColorPicker = SNew(SColorPicker);
	// TSharedRef<SColorWheel> ColorWheel = SNew(SColorWheel)
	// 	.SelectedColor_Static(&UTestFunctionLibrary::GetCurrentColor)
	// 	// .Visibility_Static(&UTestFunctionLibrary::HandleColorPickerVisibility)
	// 	.OnValueChanged_Static(&UTestFunctionLibrary::HandleOnValueChanged);
	//

	// TSharedRef<SColorPicker> ColorPicker = SNew(SColorPicker);
	// TSharedRef<SColorWheel> ColorWheel = SNew(SColorWheel)
	// 	.SelectedColor_Static(&UTestFunctionLibrary::GetCurrentColor)
	// 	.OnValueChanged_Lambda([ColorImage](FLinearColor InColor)
	// 	{
	// 		ColorRGB = InColor.HSVToLinearRGB().GetClamped(0.0f, FLT_MAX);
	// 		ColorHSV = InColor;
	// 		UE_LOG(LogTemp, Warning, TEXT("Color: %s"), *ColorRGB.ToString());
	// 		ColorImage->SetColorAndOpacity(ColorRGB);
	// 	});

	TSharedRef<SColorSpectrum> ColorSpectrum = SNew(SColorSpectrum)
		.SelectedColor_Static(&UTestFunctionLibrary::GetCurrentColor)
		.OnValueChanged_Lambda([ColorImage](FLinearColor InColor)
		{
			ColorRGB = InColor.HSVToLinearRGB().GetClamped(0.0f, FLT_MAX);
			ColorHSV = InColor;
			UE_LOG(LogTemp, Warning, TEXT("Color: %s"), *ColorRGB.ToString());
			ColorImage->SetColorAndOpacity(ColorRGB);
		});


	// TSharedRef<SSlider> ColorSlider = SNew(SSlider).Value
	

	TSharedRef<SHorizontalBox> VerticalBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.MinWidth(300.0f)
		.MaxWidth(300.0f)
		[
			SNew(SSlider).Value_Lambda([]()->float
			{
				return ColorRGB.R;
			}).OnValueChanged_Lambda([&](float InValue)
			{
				ColorRGB.R = InValue;
				ColorHSV = ColorRGB.LinearRGBToHSV();
				UE_LOG(LogTemp, Warning, TEXT("Value: %f"), ColorRGB.R);
			}).OnMouseCaptureBegin_Lambda([]()
			{
				UE_LOG(LogTemp, Warning, TEXT("MouseCaptureBegin"));
			}).OnMouseCaptureEnd_Lambda([]()
			{
				
				UE_LOG(LogTemp, Warning, TEXT("MouseCaptureEnd"));
			}).MinValue(0).MaxValue(255)		.SliderBarColor(FLinearColor::Transparent)
		.Style(&FAppStyle::Get().GetWidgetStyle<FSliderStyle>("ColorPicker.Slider")).SliderBarColor(FLinearColor::Red)
		];
		// + SHorizontalBox::Slot()
		// .VAlign(VAlign_Center)
		// .HAlign(HAlign_Center)
		// [
		// 	ColorSpectrum
		// ];
	
	// TSharedRef<SOverlay> SlatePanel = SNew(SOverlay)
	// + SOverlay::Slot()
	// 	.VAlign(VAlign_Fill)
	// 	.HAlign(HAlign_Fill)
	// 	[
	// 		SNew(SSlider).Value_Lambda([]()->float
	// 		{
	// 			return SliderValue;
	// 		}).OnValueChanged_Lambda([](float InValue)
	// 		{
	// 			SliderValue = InValue;
	// 			UE_LOG(LogTemp, Warning, TEXT("Value: %f"), SliderValue);
	// 		}).OnMouseCaptureBegin_Lambda([]()
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("MouseCaptureBegin"));
	// 		}).OnMouseCaptureEnd_Lambda([]()
	// 		{
	// 			
	// 			UE_LOG(LogTemp, Warning, TEXT("MouseCaptureEnd"));
	// 		})
	// 	]
	// 	+ SOverlay::Slot()
	// 	.VAlign(VAlign_Bottom)
	// 	.HAlign(HAlign_Center)
	// 	[
	// 		ColorImage
	// 	];
	
	
	TSharedRef<SWindow> Window = SNew(SWindow).MinWidth(400).MinHeight(400);
	Window->SetContent(VerticalBox);
	FSlateApplication::Get().AddWindow(Window);
}


FReply UTestFunctionLibrary::OnButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Button Clicked"));
	return FReply::Handled();
}

float InverseLerp(float MinValue, float MaxValue, float Alpha)
{
	float Value = MinValue + (MaxValue - MinValue) * (Alpha);
	return Value;
}
