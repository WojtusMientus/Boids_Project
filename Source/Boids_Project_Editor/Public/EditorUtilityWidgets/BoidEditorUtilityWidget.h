// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BoidEditorUtilityWidget.generated.h"


struct FGameplayTag;
struct FBoidsPlainInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPIEBeginEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPIEEndEvent);


/* 
 * Base class for Boids Editor Tool Widget.
 * Calls event on PIE simulation start & end, but also broadcasts global events for runtime BoidManagerSubsystem. 
 */
UCLASS(Blueprintable)
class BOIDS_PROJECT_EDITOR_API UBoidEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
		
	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

protected:
	
	UFUNCTION(BlueprintCallable)
	void HandleBoidColorUpdate(const FGameplayTag Tag, const FLinearColor NewColor);
	
	UFUNCTION(BlueprintCallable)
	void HandleCollisionMultiplierUpdate(float NewEnvironmentCollisionMultiplier, 
		float NewBoundsCollisionMultiplier);
	
	UFUNCTION(BlueprintCallable)
	void HandleBoidNumberUpdate(const FGameplayTag Tag, int32 CountToUpdate);
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleOnBoidNumberUpdateFinish_BP(const FGameplayTag Tag, int32 NewBoidCount);
	
	UFUNCTION(BlueprintCallable)
	void HandleOnBoidParameterChange(const FBoidsPlainInfo& BoidInfo);
	
	
	
	UPROPERTY(BlueprintAssignable)
	FOnPIEBeginEvent OnPIE_BeginEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnPIEEndEvent OnPIE_EndEvent;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsInPIE;
	
private:
	
	// Handler functions for broadcasting events at begin and PIE end.
	void HandleOnPIEBegin(bool bPIEStatus);
	void HandleOnPIEEnd(bool bPIEStatus);
	
	void HandleOnBoidNumberUpdateFinishWrapper(FGameplayTag Tag, int32 NewBoidCount);
	
	
	FDelegateHandle OnPIEBeginDelegateHandle;
	FDelegateHandle OnPIEEndDelegateHandle;
	FDelegateHandle OnBoidNumberUpdateFinishEvent;
};
