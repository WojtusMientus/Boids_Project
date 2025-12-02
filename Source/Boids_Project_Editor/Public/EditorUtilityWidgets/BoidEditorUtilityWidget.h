// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BoidEditorUtilityWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPIEBeginEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPIEEndEvent);


/* 
 * Base class for Boids Editor Tool Widget.
 * Calls event on PIE simulation start and end. 
 */
UCLASS(Blueprintable)
class BOIDS_PROJECT_EDITOR_API UBoidEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnPIEBeginEvent OnPIE_BeginEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnPIEEndEvent OnPIE_EndEvent;
	
	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

private:
	
	// Handler functions for broadcasting events at begin and PIE end.
	void HandleOnPIEBegin(bool bPIEStatus);
	void HandleOnPIEEnd(bool bPIEStatus);
	
	FDelegateHandle OnPIEBeginDelegateHandle;
	FDelegateHandle OnPIEEndDelegateHandle;
};
