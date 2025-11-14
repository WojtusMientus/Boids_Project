
#include "EditorUtilityWidgets/ColorButtonEditorUtilityWidget.h"
#include "Slate/SCustomColorPicker.h"

void UColorButtonEditorUtilityWidget::SpawnColorPicker(const FLinearColor StartingColor)
{
	FCustomColorPickerArgs SpawnArguments;
	
	SpawnArguments.StartingColor = StartingColor;
	SpawnArguments.OnColorValueChanged = FOnLinearColorValueChanged::CreateUObject(this, 
		&UColorButtonEditorUtilityWidget::HandleColorChanged);
	SpawnArguments.OnColorCommitted = FOnLinearColorValueChanged::CreateUObject(this, 
		&UColorButtonEditorUtilityWidget::HandleColorCommitted);
	SpawnArguments.OnColorCancelled = FOnLinearColorValueChanged::CreateUObject(this, 
		&UColorButtonEditorUtilityWidget::HandleColorCancelled);
	SpawnArguments.OnWindowClosed = FSimpleDelegate::CreateUObject(this,
		&UColorButtonEditorUtilityWidget::HandleColorPickerWindowClosed);
	
	SCustomColorPicker::OpenCustomColorPicker(SpawnArguments);
}

void UColorButtonEditorUtilityWidget::HandleColorChanged(FLinearColor NewColor)
{
	OnColorChanged.Broadcast(NewColor);
}

void UColorButtonEditorUtilityWidget::HandleColorCommitted(FLinearColor NewColor)
{
	OnColorCommitted.Broadcast(NewColor);
}

void UColorButtonEditorUtilityWidget::HandleColorCancelled(FLinearColor NewColor)
{
	OnColorCancelled.Broadcast(NewColor);
}

void UColorButtonEditorUtilityWidget::HandleColorPickerWindowClosed()
{
	OnColorPickerWindowClosed.Broadcast();
}
