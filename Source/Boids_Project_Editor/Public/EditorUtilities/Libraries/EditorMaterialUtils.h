// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"


class UMaterialExpressionVectorParameter;


struct FEditorMaterialUtils
{
	static UMaterialExpressionVectorParameter* CreateMaterialExpressionVectorParameter(UMaterial* ParentMaterial, 
		FLinearColor DefaultColor, FString ParameterName, FIntPoint NodePosition);
	
	static void ConnectMaterialExpressionToBaseColor(UMaterial* ParentMaterial, UMaterialExpression* Expression);

	static UMaterialInstanceConstant* CreateMaterialInstanceFrom(UMaterial* ParentMaterial, 
		const FString& InstanceName, const FString& DirectoryName);
	
private:
	
	static UMaterialExpression* CreateMaterialExpressionInternal(UMaterial* MaterialInterface, UClass* ExpressionClass, 
		FIntPoint NodePosition);
};
