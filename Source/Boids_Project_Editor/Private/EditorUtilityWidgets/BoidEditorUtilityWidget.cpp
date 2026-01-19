
#include "EditorUtilityWidgets/BoidEditorUtilityWidget.h"
#include "Utilities/BoidNumberUpdateInfo.h"
#include "Core/BoidDelegates.h"
#include "Editor/UnrealEd/Public/Editor.h"


void UBoidEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnPIEBeginDelegateHandle = FEditorDelegates::BeginPIE.AddUObject(this, &UBoidEditorUtilityWidget::HandleOnPIEBegin);
	OnPIEEndDelegateHandle = FEditorDelegates::EndPIE.AddUObject(this, &UBoidEditorUtilityWidget::HandleOnPIEEnd);
	
	if (GEditor && GEditor->IsPlayingSessionInEditor())
	{
		HandleOnPIEBegin(true);
	}
}

void UBoidEditorUtilityWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	FEditorDelegates::BeginPIE.Remove(OnPIEBeginDelegateHandle);
	FEditorDelegates::EndPIE.Remove(OnPIEEndDelegateHandle);
	BoidsDelegates::OnBoidNumberUpdateFinish.Remove(OnBoidNumberUpdateFinishEvent);
}

void UBoidEditorUtilityWidget::HandleBoidColorUpdate(const FGameplayTag Tag, const FLinearColor NewColor)
{
	BoidsDelegates::OnBoidColorUpdate.Broadcast(Tag, NewColor);
}

void UBoidEditorUtilityWidget::HandleBoidNumberUpdate(const FGameplayTag Tag, int32 CountToUpdate)
{
	BoidsDelegates::OnBoidNumberUpdate.Broadcast(FBoidNumberUpdateInfo(Tag, CountToUpdate));
}

void UBoidEditorUtilityWidget::HandleBoidForceParametersChange(const FGameplayTag Tag, float NewSeparationForce,
	float NewAlignmentForce, float NewCohesionForce, float NewOtherSpeciesMultiplier)
{
	BoidsDelegates::OnBoidForceParametersChange.Broadcast(Tag, NewSeparationForce, NewAlignmentForce, 
		NewCohesionForce, NewOtherSpeciesMultiplier);
}

void UBoidEditorUtilityWidget::HandleBoidSpatialAwarenessParameterChangeEvent(const FGameplayTag Tag,
	float NewDesiredSpeed, float NewPerceptionDistance)
{
	BoidsDelegates::OnBoidSpatialAwarenessParametersChange.Broadcast(Tag, NewDesiredSpeed, NewPerceptionDistance);
}

void UBoidEditorUtilityWidget::HandleCollisionMultiplierUpdate(const FGameplayTag Tag, 
	float EnvironmentCollisionMultiplier, float BoundsCollisionMultiplier)
{
	BoidsDelegates::OnBoidCollisionMultiplierChange.Broadcast(Tag, EnvironmentCollisionMultiplier,
		BoundsCollisionMultiplier);
}


void UBoidEditorUtilityWidget::HandleOnBoidNumberUpdateFinish_BP_Implementation(const TArray<FBoidNumberUpdateInfo>&
	UpdatedBoidNumber)
{
}

void UBoidEditorUtilityWidget::HandleOnPIEBegin(bool bPIEStatus)
{
	OnPIE_BeginEvent.Broadcast();
	bIsInPIE = true;
	
	OnBoidNumberUpdateFinishEvent = BoidsDelegates::OnBoidNumberUpdateFinish.AddUObject(this, 
		&UBoidEditorUtilityWidget::HandleOnBoidNumberUpdateFinishWrapper);
}

void UBoidEditorUtilityWidget::HandleOnPIEEnd(bool bPIEStatus)
{
	OnPIE_EndEvent.Broadcast();
	bIsInPIE = false;
	
	BoidsDelegates::OnBoidNumberUpdateFinish.Remove(OnBoidNumberUpdateFinishEvent);
}

void UBoidEditorUtilityWidget::HandleOnBoidNumberUpdateFinishWrapper(const TArray<FBoidNumberUpdateInfo>& UpdatedBoidNumber)
{
	HandleOnBoidNumberUpdateFinish_BP(UpdatedBoidNumber);
}
