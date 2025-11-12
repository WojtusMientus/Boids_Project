
#include "Slate/SCustomColorSlider.h"
#include "Widgets/Input/SSpinBox.h"


void SCustomColorSlider::Construct(const FArguments& InArgs)
{
	StartGradientColor = InArgs._StartGradientColor;
	EndGradientColor = InArgs._EndGradientColor;
	
	InitializeSliderStyle();
	InitializeSliderHandleBrush();
	InitializeBackgroundBoxBrush();
	InitializeComboBoxFontStyle();
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.MinWidth(LABEL_SIZE)
		.MaxWidth(LABEL_SIZE)
		.Padding(0.0f, 0.0f, PADDING, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(InArgs._Label)
			.TextStyle(&FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("SmallText"))
			.Justification(ETextJustify::Center)
		]
		
		+ SHorizontalBox::Slot()
		.MinWidth(SLIDER_WIDTH)
		.MaxWidth(SLIDER_WIDTH)
		[
			SAssignNew(InternalSlider, SSlider)
			.IndentHandle(false)
			.Value(InArgs._Value)
			.MinValue(InArgs._MinValue)
			.MaxValue(InArgs._MaxValue)
			.SliderBarColor(FLinearColor::Transparent)
			.SliderHandleColor(FLinearColor::Transparent)
			.Style(&SliderStyle)    
			.OnValueChanged(InArgs._OnValueChanged)
		]

		+ SHorizontalBox::Slot()
		.MinWidth(SPIN_BOX_WIDTH)
		.MaxWidth(SPIN_BOX_WIDTH)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.Padding(PADDING, 0.0f, 0.0f, 0.0f)
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
				.Style(&BackgroundBoxStyle)
				.Font(SpinBoxFontInfo)
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
	int32 CurrentPaintLayer = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
									bParentEnabled) + 1;
	
	const FVector2f SliderSize(SLIDER_WIDTH, SLIDER_HEIGHT);
	const FVector2f SliderOffset(LABEL_SIZE + PADDING, (AllottedGeometry.GetLocalSize().Y - SLIDER_HEIGHT) / 2);
	const FSlateLayoutTransform SliderLayout(SliderOffset);
	
	const FVector4f RoundedCornerRadius(2.0f, 2.0f, 2.0f, 2.0f);

	const FVector2f OutlineSliderSize(SLIDER_WIDTH + 2 * SLIDER_OUTLINE_WIDTH, 
		SLIDER_HEIGHT + 2 * SLIDER_OUTLINE_WIDTH);
	const FVector2f OutlineSliderOffset(LABEL_SIZE + PADDING - SLIDER_OUTLINE_WIDTH, 
		(AllottedGeometry.GetLocalSize().Y - SLIDER_HEIGHT) / 2 - 1);
	const FSlateLayoutTransform OutlineSliderLayout(OutlineSliderOffset);

	
	const FSlateBrush BackgroundBrush =  InternalSlider->HasMouseCapture() ? BackgroundBoxStyle.ActiveBackgroundBrush :
	(InternalSlider->IsHovered()) ? BackgroundBoxStyle.HoveredBackgroundBrush : BackgroundBoxStyle.BackgroundBrush;
	
	FSlateDrawElement::MakeBox(OutDrawElements, CurrentPaintLayer++, AllottedGeometry.ToPaintGeometry(OutlineSliderSize, OutlineSliderLayout),
		 &BackgroundBrush);
	
	TArray<FSlateGradientStop> GradientStops;
	for (int i = 0; i < GRADIENT_STOPS_COUNT; i++)
	{
		FLinearColor CalculatedGradientStopColor = FMath::Lerp(StartGradientColor, EndGradientColor, i / (GRADIENT_STOPS_COUNT - 1.0f));
		float GradientStopPosition = SLIDER_WIDTH * i / (GRADIENT_STOPS_COUNT - 1);
		GradientStops.Add(FSlateGradientStop(GradientStopPosition, CalculatedGradientStopColor));
	}
	
	FSlateDrawElement::MakeGradient(OutDrawElements, CurrentPaintLayer++, AllottedGeometry.ToPaintGeometry(SliderSize, SliderLayout),
		GradientStops, Orient_Vertical, ESlateDrawEffect::None, RoundedCornerRadius);
	
	const FVector2f SliderHandleSize(3, SLIDER_HEIGHT);
	
	const float SliderFillAmount = InternalSlider->GetValue() / InternalSlider->GetMaxValue();
	const float SliderHandleOffsetX = (SLIDER_WIDTH - SliderHandleSize.X) * SliderFillAmount;
	
	const FVector2f SliderHandleOffset(LABEL_SIZE + PADDING + SliderHandleOffsetX, (AllottedGeometry.GetLocalSize().Y - SLIDER_HEIGHT) / 2);
	const FSlateLayoutTransform SliderHandleLayout(SliderHandleOffset);
	
	FSlateDrawElement::MakeBox(OutDrawElements, CurrentPaintLayer++, AllottedGeometry.ToPaintGeometry(SliderHandleSize, SliderHandleLayout),
		&SliderHandleBrush);

	return CurrentPaintLayer;
}

void SCustomColorSlider::InitializeSliderStyle()
{
	SliderStyle = FCoreStyle::Get().GetWidgetStyle<FSliderStyle>("Slider");
	SliderStyle.NormalBarImage.DrawAs = ESlateBrushDrawType::Type::NoDrawType;
}

void SCustomColorSlider::InitializeSliderHandleBrush()
{
	SliderHandleBrush = FSlateBrush();
	SliderHandleBrush.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	SliderHandleBrush.OutlineSettings.Color = FLinearColor::Black;
	SliderHandleBrush.OutlineSettings.CornerRadii = FVector4(2.0f, 2.0f, 2.0f, 2.0f);
	SliderHandleBrush.OutlineSettings.Width = 1;
	SliderHandleBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
}

void SCustomColorSlider::InitializeBackgroundBoxBrush()
{
	BackgroundBoxStyle = FCoreStyle::Get().GetWidgetStyle<FSpinBoxStyle>("SpinBox");
	
	FSlateBrush TemporaryHelperBrush = BackgroundBoxStyle.BackgroundBrush;
	TemporaryHelperBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
	
	BackgroundBoxStyle.SetActiveFillBrush(TemporaryHelperBrush);
	BackgroundBoxStyle.SetInactiveFillBrush(TemporaryHelperBrush);
	BackgroundBoxStyle.SetHoveredFillBrush(TemporaryHelperBrush);
}

void SCustomColorSlider::InitializeComboBoxFontStyle()
{
	SpinBoxFontInfo = FCoreStyle::Get().GetFontStyle(TEXT("NormalFont"));
	SpinBoxFontInfo.Size -= 2;
}
