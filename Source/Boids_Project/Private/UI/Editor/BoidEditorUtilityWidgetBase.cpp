// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Editor/BoidEditorUtilityWidgetBase.h"
#include "UI/Slate/SCustomColorPicker.h"

void UBoidEditorUtilityWidgetBase::CreateAndPushCustomColorPickerWindow(const FLinearColor StartingColor)
{
	const FVector2D ConstructionLocation = CalculateColorPickerConstructionLocation();
	TSharedRef<SWindow> Window = CreateCustomColorPickerWindow(ConstructionLocation);
	
	if (!TestCustomColorPicker.IsValid())
	{
		TestCustomColorPicker = CreateCustomColorPicker(StartingColor, Window);
	}
	
	if (ColorPickerWindow.IsValid())
	{
		if (TSharedPtr<SWindow> ExistingWindow = ColorPickerWindow.Pin())
		{
			ExistingWindow->RequestDestroyWindow();
		}
	}
	
	TestCustomColorPicker->SetParentWindow(Window);
	TestCustomColorPicker->SetStartingColor(StartingColor);
	
	Window->SetContent(TestCustomColorPicker.ToSharedRef());
	FSlateApplication::Get().AddWindow(Window);
	Window->MoveWindowTo(ConstructionLocation);
	
	ColorPickerWindow = Window;
}

void UBoidEditorUtilityWidgetBase::TestFun()
{
	if (TestCustomColorPicker.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ColorPickerWidget is valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ColorPickerWidget is NOT valid"));
	}
}

FVector2D UBoidEditorUtilityWidgetBase::CalculateColorPickerConstructionLocation() const
{
	const FVector2D CurrentCursorPosition = FSlateApplication::Get().GetCursorPos();
	
	const FSlateRect Anchor(CurrentCursorPosition.X, CurrentCursorPosition.Y,
		CurrentCursorPosition.X, CurrentCursorPosition.Y);

	const FVector2D WindowSize = SCustomColorPicker::DEFAULT_WINDOW_SIZE + AdditionalColorPickerWindowSpawnPadding;
	return FSlateApplication::Get().CalculatePopupWindowPosition(Anchor,
		WindowSize, true, FVector2D::ZeroVector, Orient_Horizontal);
}

TSharedRef<SCustomColorPicker> UBoidEditorUtilityWidgetBase::CreateCustomColorPicker(const FLinearColor StartingColor,
	const TSharedRef<SWindow>& ParentWindow) const
{
	return  SNew(SCustomColorPicker)
		.OldColor(StartingColor)
		.ParentWindow(ParentWindow)
		.OnColorValueChanged_UObject(this, &UBoidEditorUtilityWidgetBase::HandleColorChanged)
		.OnColorCommitted_UObject(this, &UBoidEditorUtilityWidgetBase::HandleColorCommitted)
		.OnColorCancelled_UObject(this, &UBoidEditorUtilityWidgetBase::HandleColorCancelled);
}

TSharedRef<SWindow> UBoidEditorUtilityWidgetBase::CreateCustomColorPickerWindow(const FVector2D SpawnLocation) const
{
	return SNew(SWindow)
		.Title(FText::FromString("Custom Color Picker"))
		.ScreenPosition(SpawnLocation)
		.SizingRule(ESizingRule::Autosized)
		.SupportsMinimize(false)
		.SupportsMaximize(false);
}

void UBoidEditorUtilityWidgetBase::HandleColorChanged(FLinearColor NewColor) const
{
	OnColorChanged.Broadcast(NewColor);
}

void UBoidEditorUtilityWidgetBase::HandleColorCommitted(FLinearColor NewColor) const
{
	OnColorCommitted.Broadcast(NewColor);
}

void UBoidEditorUtilityWidgetBase::HandleColorCancelled(FLinearColor NewColor) const
{
	OnColorCancelled.Broadcast(NewColor);
}