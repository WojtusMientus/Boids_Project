
#include "EditorUtilityWidgets/BoidEditorUtilityWidgetBase.h"
#include "Slate/SCustomColorPicker.h"

void UBoidEditorUtilityWidgetBase::SpawnColorPicker(const FLinearColor StartingColor)
{
	FCustomColorPickerArgs SpawnArguments;
	
	SpawnArguments.StartingColor = StartingColor;
	SpawnArguments.OnColorValueChanged = FOnLinearColorValueChanged::CreateUObject(this, 
		&UBoidEditorUtilityWidgetBase::HandleColorChanged);
	SpawnArguments.OnColorCommitted = FOnLinearColorValueChanged::CreateUObject(this, 
		&UBoidEditorUtilityWidgetBase::HandleColorCommitted);
	SpawnArguments.OnColorCancelled = FOnLinearColorValueChanged::CreateUObject(this, 
		&UBoidEditorUtilityWidgetBase::HandleColorCancelled);
	
	SCustomColorPicker::OpenCustomColorPicker(SpawnArguments);
}

void UBoidEditorUtilityWidgetBase::HandleColorChanged(FLinearColor NewColor)
{
	OnColorChanged.Broadcast(NewColor);
}

void UBoidEditorUtilityWidgetBase::HandleColorCommitted(FLinearColor NewColor)
{
	OnColorCommitted.Broadcast(NewColor);
}

void UBoidEditorUtilityWidgetBase::HandleColorCancelled(FLinearColor NewColor)
{
	OnColorCancelled.Broadcast(NewColor);
}