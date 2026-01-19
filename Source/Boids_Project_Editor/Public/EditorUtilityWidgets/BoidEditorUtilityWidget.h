// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BoidEditorUtilityWidget.generated.h"

struct FBoidNumberUpdateInfo;
struct FGameplayTag;
struct FBoidsSpeciesPlainInfo;


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
	void HandleBoidNumberUpdate(const FGameplayTag Tag, int32 CountToUpdate);
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleOnBoidNumberUpdateFinish_BP(const TArray<FBoidNumberUpdateInfo>& UpdatedBoidNumber);
	
	
	UFUNCTION(BlueprintCallable)
	void HandleBoidForceParametersChange(const FGameplayTag Tag, float NewSeparationForce, float NewAlignmentForce,
		float NewCohesionForce, float NewOtherSpeciesMultiplier);
	
	UFUNCTION(BlueprintCallable)
	void HandleBoidSpatialAwarenessParameterChangeEvent(const FGameplayTag Tag, float NewDesiredSpeed,
		float NewPerceptionDistance);
	
	UFUNCTION(BlueprintCallable)
	void HandleCollisionMultiplierUpdate(const FGameplayTag Tag, float EnvironmentCollisionMultiplier, 
		float BoundsCollisionMultiplier);
	
	
	
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
	
	void HandleOnBoidNumberUpdateFinishWrapper(const TArray<FBoidNumberUpdateInfo>& UpdatedBoidNumber);
	
	
	FDelegateHandle OnPIEBeginDelegateHandle;
	FDelegateHandle OnPIEEndDelegateHandle;
	FDelegateHandle OnBoidNumberUpdateFinishEvent;
};
