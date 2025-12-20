
#include "EditorUtilityWidgets/BoidEditorUtilityWidget.h"

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

void UBoidEditorUtilityWidget::HandleCollisionMultiplierUpdate(float NewEnvironmentCollisionMultiplier,
	float NewBoundsCollisionMultiplier)
{
	BoidsDelegates::OnCollisionMultiplierUpdate.Broadcast(NewEnvironmentCollisionMultiplier,
		NewBoundsCollisionMultiplier);
}

void UBoidEditorUtilityWidget::HandleBoidNumberUpdate(const FGameplayTag Tag, int32 CountToUpdate)
{
	BoidsDelegates::OnBoidNumberUpdate.Broadcast(Tag, CountToUpdate);
}

void UBoidEditorUtilityWidget::HandleOnBoidParameterChange(const FBoidsPlainInfo& BoidInfo)
{
	BoidsDelegates::OnBoidParameterChange.Broadcast(BoidInfo);
}

void UBoidEditorUtilityWidget::HandleOnBoidNumberUpdateFinish_BP_Implementation(FGameplayTag Tag, int32 NewBoidCount)
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

void UBoidEditorUtilityWidget::HandleOnBoidNumberUpdateFinishWrapper(FGameplayTag Tag, int32 NewBoidCount)
{
	HandleOnBoidNumberUpdateFinish_BP_Implementation(Tag, NewBoidCount);
}
