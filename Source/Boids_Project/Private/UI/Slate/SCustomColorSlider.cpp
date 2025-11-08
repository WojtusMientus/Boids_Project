
#include "UI/Slate/SCustomColorSlider.h"
#include "Widgets/Input/SSpinBox.h"



void SCustomColorSlider::Construct(const FArguments& InArgs)
{
	StartGradientColor = InArgs._StartGradientColor;
	EndGradientColor = InArgs._EndGradientColor;

	SpinBoxBrush = MakeUnique<FSpinBoxStyle>(FCoreStyle::Get().GetWidgetStyle<FSpinBoxStyle>("SpinBox"));

	SliderStyle = MakeUnique<FSliderStyle>(FCoreStyle::Get().GetWidgetStyle<FSliderStyle>("Slider"));
	SliderStyle->NormalBarImage.DrawAs = ESlateBrushDrawType::Type::NoDrawType;

	SelectorBrush = MakeUnique<FSlateBrush>();
	SelectorBrush->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	SelectorBrush->OutlineSettings.Color = FLinearColor::Black;
	SelectorBrush->OutlineSettings.CornerRadii = FVector4(2.0f, 2.0f, 2.0f, 2.0f);
	SelectorBrush->OutlineSettings.Width = 1;
	SelectorBrush->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		
	FSlateBrush TempBrush = SpinBoxBrush->BackgroundBrush;
	TempBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	
	SpinBoxBrush->SetActiveFillBrush(TempBrush);
	SpinBoxBrush->SetInactiveFillBrush(TempBrush);
	SpinBoxBrush->SetHoveredFillBrush(TempBrush);

	ComboBoxStyle = MakeUnique<FSlateFontInfo>(FCoreStyle::Get().GetFontStyle(TEXT("NormalFont")));
	ComboBoxStyle->Size -= 2;
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.MinWidth(LabelSize)
		.MaxWidth(LabelSize)
		.Padding(0.0f, 0.0f, Padding, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(InArgs._Label)
			.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("SmallText"))
			.Justification(ETextJustify::Center)
		]
		
		+ SHorizontalBox::Slot()
		.MinWidth(SliderWidth)
		.MaxWidth(SliderWidth)
		[
			SAssignNew(InternalSlider, SSlider)
			.IndentHandle(false)
			.Value(InArgs._Value)
			.MinValue(InArgs._MinValue)
			.MaxValue(InArgs._MaxValue)
			.SliderBarColor(FLinearColor::Transparent)
			.SliderHandleColor(FLinearColor::Transparent)
			.Style(SliderStyle.Get())    
			.OnValueChanged(InArgs._OnValueChanged)
		]

		+ SHorizontalBox::Slot()
		.MinWidth(SpinBoxWidth)
		.MaxWidth(SpinBoxWidth)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.Padding(Padding, 0.0f, 0.0f, 0.0f)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.MinHeight(20)
			.MaxHeight(20)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SSpinBox<float>)
				.Value(InArgs._Value)
				.MinValue(InArgs._MinValue)
				.MaxValue(InArgs._MaxValue)
				.MaxFractionalDigits(3)
				.Delta(0.001f)
				.Style(SpinBoxBrush.Get())
				.Font(*ComboBoxStyle.Get())
				.ContentPadding(FMargin())
				.OnValueChanged(InArgs._OnValueChanged)
			]
		]
	];
}

int32 SCustomColorSlider::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 CurrentLayer = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
									bParentEnabled) + 1;
	
	const FVector2f SliderSize(SliderWidth, SliderHeight);
	const FVector2f SliderOffset(LabelSize + Padding, (AllottedGeometry.GetLocalSize().Y - SliderHeight) / 2);
	const FSlateLayoutTransform SliderLayout(SliderOffset);
	
	const FVector4f RoundedCornerRadius(2.0f, 2.0f, 2.0f, 2.0f);

	const FVector2f OutlineSliderSize(SliderWidth + 2, SliderHeight + 2);
	const FVector2f OutlineSliderOffset(LabelSize + Padding - 1, (AllottedGeometry.GetLocalSize().Y - SliderHeight) / 2 - 1);
	const FSlateLayoutTransform OutlineSliderLayout(OutlineSliderOffset);

	
	const FSlateBrush BorderBrush =  InternalSlider->HasMouseCapture() ? SpinBoxBrush->ActiveBackgroundBrush :
	(InternalSlider->IsHovered()) ? SpinBoxBrush->HoveredBackgroundBrush : SpinBoxBrush->BackgroundBrush;
	
	FSlateDrawElement::MakeBox(OutDrawElements, CurrentLayer++, AllottedGeometry.ToPaintGeometry(OutlineSliderSize, OutlineSliderLayout),
		 &BorderBrush);
	
	TArray<FSlateGradientStop> GradientStops;
	float GradientStopPosition;
	for (int i = 0; i < NumberOfGradientStops; i++)
	{
		FLinearColor CalculatedGradientStopColor = FMath::Lerp(StartGradientColor, EndGradientColor, i / (NumberOfGradientStops - 1.0f));
		GradientStopPosition = SliderWidth * i / (NumberOfGradientStops - 1);
		GradientStops.Add(FSlateGradientStop(GradientStopPosition, CalculatedGradientStopColor));
	}
	
	FSlateDrawElement::MakeGradient(OutDrawElements, CurrentLayer++, AllottedGeometry.ToPaintGeometry(SliderSize, SliderLayout),
		GradientStops, Orient_Vertical, ESlateDrawEffect::None, RoundedCornerRadius);
	
	const FVector2f SliderHandleSize(3, SliderHeight);
	
	const float SliderFillAmount = InternalSlider->GetValue() / InternalSlider->GetMaxValue();
	const float SliderHandleOffsetX = (SliderWidth - SliderHandleSize.X) * SliderFillAmount;
	
	const FVector2f SliderHandleOffset(LabelSize + Padding + SliderHandleOffsetX, (AllottedGeometry.GetLocalSize().Y - SliderHeight) / 2);
	const FSlateLayoutTransform SliderHandleLayout(SliderHandleOffset);
	
	FSlateDrawElement::MakeBox(OutDrawElements, CurrentLayer++, AllottedGeometry.ToPaintGeometry(SliderHandleSize, SliderHandleLayout),
		SelectorBrush.Get());

	return CurrentLayer;
}