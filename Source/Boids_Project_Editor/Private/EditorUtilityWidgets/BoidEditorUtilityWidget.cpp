
#include "EditorUtilityWidgets/BoidEditorUtilityWidget.h"
#include "Editor/UnrealEd/Public/Editor.h"


void UBoidEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnPIEBeginDelegateHandle = FEditorDelegates::BeginPIE.AddUObject(this, &UBoidEditorUtilityWidget::HandleOnPIEBegin);
	OnPIEEndDelegateHandle = FEditorDelegates::EndPIE.AddUObject(this, &UBoidEditorUtilityWidget::HandleOnPIEEnd);
}

void UBoidEditorUtilityWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	FEditorDelegates::BeginPIE.Remove(OnPIEBeginDelegateHandle);
	FEditorDelegates::EndPIE.Remove(OnPIEEndDelegateHandle);
}

void UBoidEditorUtilityWidget::HandleOnPIEBegin(bool bPIEStatus)
{
	OnPIE_BeginEvent.Broadcast();
}

void UBoidEditorUtilityWidget::HandleOnPIEEnd(bool bPIEStatus)
{
	OnPIE_EndEvent.Broadcast();
}
