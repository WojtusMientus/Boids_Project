
#include "Core/Managers/EditorBoidDataManager.h"
#include "GameplayTagsManager.h"
#include "Utilities/BoidConstants.h"
#include "DataAssets/BoidsData.h"
#include "DataAssets/BoundsData.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "EditorUtilities/Libraries/EditorAssetUtils.h"
#include "EditorUtilities/Libraries/EditorMaterialUtils.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Utilities/Libraries/RuntimeAssetUtils.h"


void FEditorBoidDataManager::InitializeBoidSimulationData(FCollisionBoundsPlainInfo& OutBoundsData,
                                                          TMap<FGameplayTag, FBoidsPlainInfo>& OutBoidsData)
{
	EnsureNecessarySimulationData();
	OutBoundsData = GetCopyOfBoundsData();
	OutBoidsData = GetCopyOfBoidsData();
}

void FEditorBoidDataManager::SaveBoundsData(const FCollisionBoundsPlainInfo& CollisionBoundsData, 
	const TArray<FEnvironmentCollisionCellData>& CalculatedCollisionData)
{	
	ENSURE_ALWAYS_RETURN(IsValid(LoadedBoundsDataAsset))
	
	LoadedBoundsDataAsset->OverwritePlainData(CollisionBoundsData, CalculatedCollisionData);
	FEditorAssetUtils::SaveAsset(LoadedBoundsDataAsset);
}

void FEditorBoidDataManager::SaveBoidsData(const FBoidsPlainInfo& BoidsDataToSave)
{
	UBoidsData* BoidsAssetToSave = LoadedBoidsDataAssets[BoidsDataToSave.Type];
	ENSURE_ALWAYS_RETURN(IsValid(BoidsAssetToSave))
	
	BoidsAssetToSave->OverwritePlainData(BoidsDataToSave);
	FEditorAssetUtils::SaveAsset(BoidsAssetToSave);
}

void FEditorBoidDataManager::SaveAllBoidsData(const TMap<FGameplayTag, FBoidsPlainInfo>& AllBoidsDataToSave)
{
	for (const TPair<FGameplayTag, FBoidsPlainInfo>& Pair: AllBoidsDataToSave)
	{
		SaveBoidsData(Pair.Value);
	}
}

void FEditorBoidDataManager::EnsureNecessarySimulationData()
{
	EnsureBoidSpeciesDataAssets();	
	EnsureBoundsDataAssets();
	EnsureMaterialAssets();
}

FCollisionBoundsPlainInfo FEditorBoidDataManager::GetCopyOfBoundsData() const
{
	return FCollisionBoundsPlainInfo(LoadedBoundsDataAsset);
}

TMap<FGameplayTag, FBoidsPlainInfo> FEditorBoidDataManager::GetCopyOfBoidsData() const
{
	TMap<FGameplayTag, FBoidsPlainInfo> BoidsDataCopy;
	
	for (const TPair<FGameplayTag, UBoidsData*>& Pair: LoadedBoidsDataAssets)
	{
		const UBoidsData* OriginalBoidData = Pair.Value;
		ENSURE_ALWAYS_CONTINUE(IsValid(OriginalBoidData))
		
		FBoidsPlainInfo BoidSpeciesDataCopy = OriginalBoidData->GetPlainDataInfo();
		BoidsDataCopy.Add(OriginalBoidData->Type, BoidSpeciesDataCopy);
	}
	
	return BoidsDataCopy;
}

void FEditorBoidDataManager::EnsureBoidSpeciesDataAssets()
{
	EnsureBoidsDataDirectoryExist();
		
	TArray<FGameplayTag> BoidSpeciesTags = GetBoidsSpeciesTags();
	FilterAlreadyCreatedBoidDataAssets(BoidSpeciesTags);
	
	for (const FGameplayTag BoidTag: BoidSpeciesTags)
	{
		CreateBoidSpeciesDataWithTag(BoidTag);
	}	
}

void FEditorBoidDataManager::EnsureBoundsDataAssets()
{
	EnsureBoundsDataDirectoryExist();
	
	TArray<FAssetData> BoundsDataAssets = FRuntimeAssetUtils::GetFilteredAssetDataByClassAndPath(UBoundsData::StaticClass(), 
	FBoidConstants::Paths::BoundsPath, false);
	
	if (BoundsDataAssets.Num() == 0)
	{
		LoadedBoundsDataAsset = FEditorAssetUtils::CreateAsset<UBoundsData>(FBoidConstants::AssetNames::BoundsName, 
			FBoidConstants::Paths::BoundsPath);
		FEditorAssetUtils::SaveAsset(LoadedBoundsDataAsset);
	}
	else
	{
		LoadedBoundsDataAsset = Cast<UBoundsData>(BoundsDataAssets[0].GetAsset());
	}
}

void FEditorBoidDataManager::EnsureMaterialAssets()
{
	EnsureMaterialDataDirectoryExist();
	EnsureBaseMaterialAsset();	
	EnsureMaterialInstanceAsset();
}

void FEditorBoidDataManager::EnsureBaseMaterialAsset()
{
	TArray<FAssetData> MaterialAssets = FRuntimeAssetUtils::GetFilteredAssetDataByClassAndPath(UMaterial::StaticClass(), 
	FBoidConstants::Paths::MaterialsPath, false);
	
	if (MaterialAssets.Num() == 0)
	{
		LoadedMaterialAsset = FEditorAssetUtils::CreateAsset<UMaterial>(FBoidConstants::AssetNames::MaterialName, 
			FBoidConstants::Paths::MaterialsPath);
		
		UMaterialExpressionVectorParameter* ColorParameter = FEditorMaterialUtils::
			CreateMaterialExpressionVectorParameter(LoadedMaterialAsset, FLinearColor::Red, 
				FBoidConstants::BoidMaterialColorParameter, FIntPoint(-200, 25));
		FEditorMaterialUtils::ConnectMaterialExpressionToBaseColor(LoadedMaterialAsset, ColorParameter);
		
		FEditorAssetUtils::SaveAsset(LoadedMaterialAsset);
	}
	else
	{
		LoadedMaterialAsset = Cast<UMaterial>(MaterialAssets[0].GetAsset());
	}
}

void FEditorBoidDataManager::EnsureMaterialInstanceAsset()
{
	TArray<FAssetData> MaterialInstanceConstantAssets = FRuntimeAssetUtils::GetFilteredAssetDataByClassAndPath(
	UMaterialInstanceConstant::StaticClass(), FBoidConstants::Paths::MaterialsPath, false);
	
	if (MaterialInstanceConstantAssets.Num() == 0)
	{
		UMaterialInstanceConstant* CreatedMaterialInstance = FEditorMaterialUtils::CreateMaterialInstanceFrom(
			LoadedMaterialAsset, FBoidConstants::AssetNames::MaterialInstanceName, 
			FBoidConstants::Paths::MaterialsPath);
		FEditorAssetUtils::SaveAsset(CreatedMaterialInstance);
	}
}


void FEditorBoidDataManager::EnsureBoidsDataDirectoryExist() const
{
	FEditorAssetUtils::EnsureDirectoryExist(FBoidConstants::Paths::SpeciesPath);
}

void FEditorBoidDataManager::EnsureBoundsDataDirectoryExist() const
{
	FEditorAssetUtils::EnsureDirectoryExist(FBoidConstants::Paths::BoundsPath);
}

void FEditorBoidDataManager::EnsureMaterialDataDirectoryExist() const
{
	FEditorAssetUtils::EnsureDirectoryExist(FBoidConstants::Paths::MaterialsPath);
}

void FEditorBoidDataManager::FilterAlreadyCreatedBoidDataAssets(TArray<FGameplayTag>& BoidSpeciesTags)
{
	TArray<FAssetData> BoidSpeciesDataAssets = FRuntimeAssetUtils::GetFilteredAssetDataByClassAndPath(
		UBoidsData::StaticClass(), FBoidConstants::Paths::SpeciesPath, false);
	
	for (const FAssetData& Asset: BoidSpeciesDataAssets)
	{
		UObject* FoundAsset = Asset.GetAsset();
		
		if (UBoidsData* FoundBoidsDataAsset = Cast<UBoidsData>(FoundAsset))
		{
			if (BoidSpeciesTags.Contains(FoundBoidsDataAsset->Type))
			{
				BoidSpeciesTags.Remove(FoundBoidsDataAsset->Type);
				LoadedBoidsDataAssets.Add(FoundBoidsDataAsset->Type, FoundBoidsDataAsset);
			}
		}
	}
}

TArray<FGameplayTag> FEditorBoidDataManager::GetBoidsSpeciesTags() const
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	FGameplayTag FoundTag = GameplayTagsManager.RequestGameplayTag(FName(FBoidConstants::SpeciesTagName));
	FGameplayTagContainer TagContainer = GameplayTagsManager.RequestGameplayTagChildren(FoundTag); 
	
	return TagContainer.GetGameplayTagArray();
}

void FEditorBoidDataManager::CreateBoidSpeciesDataWithTag(const FGameplayTag Tag)
{	
	const FString AssetName = FBoidConstants::AssetNames::SpeciesPrefix + Tag.GetTagLeafName().ToString();
		
	if (UBoidsData* CreatedAsset = FEditorAssetUtils::CreateAsset<UBoidsData>(AssetName, 
		FBoidConstants::Paths::SpeciesPath))
	{
		CreatedAsset->Type = Tag;
		FEditorAssetUtils::SaveAsset(CreatedAsset);
		LoadedBoidsDataAssets.Add(CreatedAsset->Type, CreatedAsset);
	}
}
