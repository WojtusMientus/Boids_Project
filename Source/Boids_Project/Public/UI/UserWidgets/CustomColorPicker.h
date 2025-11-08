
#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Blueprint/UserWidget.h"
#include "CustomColorPicker.generated.h"

class SCustomColorPicker;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorChangedEvent, FLinearColor, NewColor);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCommittedEvent, FLinearColor, NewColor);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColorCancelledEvent, FLinearColor, NewColor);

UCLASS()
class BOIDS_PROJECT_API UCustomColorPicker : public UWidget
{
	GENERATED_BODY()

public:

// 	UPROPERTY(Blueprintassignable)
// 	FOnColorChangedEvent OnColorChanged;
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnColorCommittedEvent OnColorCommitted;
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnColorCancelledEvent OnColorCancelled;
// 	
// 	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
// 	
// protected:
// 	virtual TSharedRef<SWidget> RebuildWidget() override;
// 	
// private:
// 	
// 	void HandleColorChanged(FLinearColor NewColor) const;
// 	void HandleColorCommitted(FLinearColor NewColor) const;
// 	void HandleColorCancelled(FLinearColor NewColor) const;
//
// 	TSharedPtr<SCustomColorPicker> CustomColorPicker;
};
