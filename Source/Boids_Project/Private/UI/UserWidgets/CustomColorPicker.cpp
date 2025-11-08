
#include "UI/UserWidgets/CustomColorPicker.h"

#include "UI/Slate/SCustomColorPicker.h"

//
// void UCustomColorPicker::ReleaseSlateResources(bool bReleaseChildren)
// {
// 	Super::ReleaseSlateResources(bReleaseChildren);
// }
//
// TSharedRef<SWidget> UCustomColorPicker::RebuildWidget()
// {
// 	CustomColorPicker = SNew(SCustomColorPicker)
// 		.OldColor(FLinearColor::White)
// 		.OnColorValueChanged_UObject(this, &UCustomColorPicker::HandleColorChanged)
// 		.OnColorCommitted_UObject(this, &UCustomColorPicker::HandleColorCommitted)
// 		.OnColorCancelled_UObject(this, &UCustomColorPicker::HandleColorCancelled);
// 	
// 	return CustomColorPicker.ToSharedRef();
// }
//
// void UCustomColorPicker::HandleColorChanged(FLinearColor NewColor) const
// {
// 	OnColorChanged.Broadcast(NewColor);
// }
//
// void UCustomColorPicker::HandleColorCommitted(FLinearColor NewColor) const
// {
// 	OnColorCommitted.Broadcast(NewColor);
// }
//
// void UCustomColorPicker::HandleColorCancelled(FLinearColor NewColor) const
// {
// 	OnColorCancelled.Broadcast(NewColor);
// }
