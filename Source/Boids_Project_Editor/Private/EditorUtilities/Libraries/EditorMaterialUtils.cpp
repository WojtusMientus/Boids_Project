
#include "EditorUtilities/Libraries/EditorMaterialUtils.h"
#include "MaterialEditingLibrary.h"
#include "EditorUtilities/Libraries/EditorAssetUtils.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialExpressionVectorParameter.h"


UMaterialExpressionVectorParameter* FEditorMaterialUtils::CreateMaterialExpressionVectorParameter(
	UMaterial* ParentMaterial, FLinearColor DefaultColor, FString ParameterName, FIntPoint NodePosition)
{
	UMaterialExpressionVectorParameter* VectorParameter = Cast<UMaterialExpressionVectorParameter>
	(CreateMaterialExpressionInternal(ParentMaterial, UMaterialExpressionVectorParameter::StaticClass(), NodePosition));
	
	if (!VectorParameter)
	{
		return nullptr;
	}
	
	VectorParameter->ParameterName = FName(ParameterName);
	VectorParameter->DefaultValue = DefaultColor;
	
	return VectorParameter;	
}

void FEditorMaterialUtils::ConnectMaterialExpressionToBaseColor(UMaterial* ParentMaterial, UMaterialExpression* Expression)
{
	ParentMaterial->GetEditorOnlyData()->BaseColor.Connect(0, Expression);
	UMaterialEditingLibrary::RecompileMaterial(ParentMaterial);
}

UMaterialInstanceConstant* FEditorMaterialUtils::CreateMaterialInstanceFrom(UMaterial* ParentMaterial, 
	const FString& InstanceName, const FString& DirectoryName)
{
	if (!ParentMaterial)
	{
		return nullptr;
	}
	
	UMaterialInstanceConstantFactoryNew* MaterialInstanceFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
	MaterialInstanceFactory->InitialParent = ParentMaterial;
		
	return FEditorAssetUtils::CreateAsset<UMaterialInstanceConstant>(InstanceName, DirectoryName, 
		MaterialInstanceFactory);
}

UMaterialExpression* FEditorMaterialUtils::CreateMaterialExpressionInternal(UMaterial* ParentMaterial,
                                                                            UClass* ExpressionClass, FIntPoint NodePosition)
{
	UMaterialExpression* CreatedMaterial =  UMaterialEditingLibrary::CreateMaterialExpression(ParentMaterial, 
		ExpressionClass, NodePosition.X, NodePosition.Y);
	ParentMaterial->AddExpressionParameter(CreatedMaterial, ParentMaterial->EditorParameters);
	
	return CreatedMaterial;
}
