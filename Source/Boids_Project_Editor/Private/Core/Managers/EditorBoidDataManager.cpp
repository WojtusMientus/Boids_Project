
#include "Core/Managers/EditorBoidDataManager.h"
#include "AssetToolsModule.h"
#include "EditorAssetLibrary.h"
#include "FileHelpers.h"
#include "GameplayTagsManager.h"
#include "IAssetTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/BoidDataHelpers.h"
#include "DataAssets/BoidsData.h"
#include "DataAssets/BoundsData.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "Materials/MaterialInstanceConstant.h"


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
	ENSURE_BOUNDS_DATA_ASSET()
	
	LoadedBoundsDataAsset->OverwritePlainData(CollisionBoundsData, CalculatedCollisionData);
	SaveAsset(LoadedBoundsDataAsset);
}

void FEditorBoidDataManager::SaveBoidsData(const FBoidsPlainInfo& BoidsDataToSave)
{
	UBoidsData* BoidsAssetToSave = LoadedBoidsDataAssets[BoidsDataToSave.Type];
	ENSURE_BOIDS_DATA_ASSET(IsValid(BoidsAssetToSave))
	
	BoidsAssetToSave->OverwritePlainData(BoidsDataToSave);
	SaveAsset(BoidsAssetToSave);
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
	EnsureDataFactoriesExist();
	EnsureBoidSpeciesDataAssets();	
	EnsureBoundsDataAssets();
	EnsureMaterialAssets();
}

FCollisionBoundsPlainInfo FEditorBoidDataManager::GetCopyOfBoundsData() const
{
	FCollisionBoundsPlainInfo BoundsDataCopy;
	ENSURE_BOUNDS_DATA_ASSET_RETURN_VALUE()
	
	BoundsDataCopy.OverwriteData(LoadedBoundsDataAsset);
	return BoundsDataCopy;
}

TMap<FGameplayTag, FBoidsPlainInfo> FEditorBoidDataManager::GetCopyOfBoidsData() const
{
	TMap<FGameplayTag, FBoidsPlainInfo> BoidsDataCopy;
	
	for (const TPair<FGameplayTag, UBoidsData*>& Pair: LoadedBoidsDataAssets)
	{
		FBoidsPlainInfo BoidSpeciesDataCopy;
		const UBoidsData* OriginalBoidData = Pair.Value;
		
		ENSURE_BOIDS_DATA_ASSET_CONTINUE(IsValid(OriginalBoidData))
		
		BoidSpeciesDataCopy.OverwriteData(OriginalBoidData);
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
	
	TArray<FAssetData> BoundsDataAssets = GetFilteredAssetDataByClassAndPath(UBoundsData::StaticClass(), 
	BoidDataHelpers::GetBoidsBoundsDirectoryName());
	
	if (BoundsDataAssets.Num() == 0)
	{
		CreateBoundsData();
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
	TArray<FAssetData> MaterialAssets = GetFilteredAssetDataByClassAndPath(UMaterial::StaticClass(), 
	BoidDataHelpers::GetBoidsMaterialsDirectoryName());
	
	if (MaterialAssets.Num() == 0)
	{
		CreateAsset(MATERIAL_DATA_NAME, BoidDataHelpers::GetBoidsMaterialsDirectoryName(), UMaterial::StaticClass(), MaterialFactory);
	}
}

void FEditorBoidDataManager::EnsureMaterialInstanceAsset()
{
	TArray<FAssetData> MaterialInstanceConstantAssets = GetFilteredAssetDataByClassAndPath(
	UMaterialInstanceConstant::StaticClass(), BoidDataHelpers::GetBoidsMaterialsDirectoryName());
	
	if (MaterialInstanceConstantAssets.Num() == 0)
	{
		CreateAsset(MATERIAL_INSTANCE_DATA_NAME, BoidDataHelpers::GetBoidsMaterialsDirectoryName(),
			UMaterialInstanceConstant::StaticClass(), MaterialConstantFactory);
	}
}

void FEditorBoidDataManager::EnsureDirectoryExist(const FString& DirectoryName) const
{
	if (!UEditorAssetLibrary::DoesDirectoryExist(DirectoryName))
	{
		UEditorAssetLibrary::MakeDirectory(DirectoryName);
	}
}

void FEditorBoidDataManager::EnsureBoidsDataDirectoryExist() const
{
	EnsureDirectoryExist(BoidDataHelpers::GetBoidsSpeciesDataDirectoryName());
}

void FEditorBoidDataManager::EnsureBoundsDataDirectoryExist() const
{
	EnsureDirectoryExist(BoidDataHelpers::GetBoidsBoundsDirectoryName());
}

void FEditorBoidDataManager::EnsureMaterialDataDirectoryExist() const
{
	EnsureDirectoryExist(BoidDataHelpers::GetBoidsMaterialsDirectoryName());
}

void FEditorBoidDataManager::FilterAlreadyCreatedBoidDataAssets(TArray<FGameplayTag>& BoidSpeciesTags)
{
	TArray<FAssetData> BoidSpeciesDataAssets = GetFilteredAssetDataByClassAndPath(UBoidsData::StaticClass(), 
	BoidDataHelpers::GetBoidsSpeciesDataDirectoryName());
	
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

TArray<FAssetData> FEditorBoidDataManager::GetFilteredAssetDataByClassAndPath(UClass* ClassToFilter, 
	const FString& PathToFilter) const
{
	IAssetRegistry& AssetRegistry  = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	TArray<FAssetData> FilteredAssetDatas;
	
	FARFilter Filter = BoidDataHelpers::GetFilterByClassAndPath(ClassToFilter, PathToFilter);
	AssetRegistry.GetAssets(Filter, FilteredAssetDatas);
	
	return FilteredAssetDatas;
}

TArray<FGameplayTag> FEditorBoidDataManager::GetBoidsSpeciesTags() const
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	FGameplayTag FoundTag = GameplayTagsManager.RequestGameplayTag(FName(BoidDataHelpers::GetBoidSpeciesTagName()));
	FGameplayTagContainer TagContainer = GameplayTagsManager.RequestGameplayTagChildren(FoundTag); 
	
	return TagContainer.GetGameplayTagArray();
}

void FEditorBoidDataManager::CreateBoidSpeciesDataWithTag(const FGameplayTag Tag)
{	
	const FString AssetName = BOIDS_SPECIES_DATA_NAME_PREFIX + Tag.GetTagLeafName().ToString();
		
	if (UBoidsData* CreatedAsset = CreateBoidsData(AssetName))
	{
		CreatedAsset->Type = Tag;
		SaveAsset(CreatedAsset);
		LoadedBoidsDataAssets.Add(CreatedAsset->Type, CreatedAsset);
	}
}

UBoidsData* FEditorBoidDataManager::CreateBoidsData(const FString& AssetName)
{
	UObject* CreatedAsset = CreateAsset(AssetName, BoidDataHelpers::GetBoidsSpeciesDataDirectoryName(),
		UBoidsData::StaticClass(), DataAssetFactory);
	return Cast<UBoidsData>(CreatedAsset);
}

void FEditorBoidDataManager::CreateBoundsData()
{
	const FString AssetDirectory = BoidDataHelpers::GetBoidsBoundsDirectoryName();
		
	LoadedBoundsDataAsset = Cast<UBoundsData>(CreateAsset(BOUNDS_DATA_NAME, AssetDirectory, 
		UBoundsData::StaticClass(), DataAssetFactory));
}

UObject* FEditorBoidDataManager::CreateAsset(const FString& AssetName, const FString& AssetDirectory, 
                                             UClass* AssetClass, UFactory* AssetFactory)
{
	if (AssetFactory == nullptr)
	{
		return nullptr;
	}
	
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	UObject* CreatedAsset = AssetTools.CreateAsset(AssetName, AssetDirectory, AssetClass, AssetFactory);
	
	if (CreatedAsset == nullptr)
	{
		return nullptr;
	}
	
	SaveAsset(CreatedAsset);
	return CreatedAsset;
}

void FEditorBoidDataManager::SaveAsset(UObject* AssetToSave)
{
	UPackage* PackageToSave = AssetToSave->GetOutermost();
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(PackageToSave);
	
	ENSURE_ALWAYS_MESSAGE_RETURN(IsValid(PackageToSave), "Saving Package Not Valid.")
	
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
}

void FEditorBoidDataManager::EnsureDataFactoriesExist()
{	
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	for (UFactory* Factory: AssetTools.GetNewAssetFactories())
	{
		if (IsValid(DataAssetFactory) && IsValid(MaterialFactory) && IsValid(MaterialConstantFactory))
		{
			return;
		}
		
		if (IsViableFactoryClass(Factory, UDataAsset::StaticClass()))
		{
			DataAssetFactory = Factory;
		}
		else if (IsViableFactoryClass(Factory, UMaterial::StaticClass()))
		{
			MaterialFactory = Factory;
		}
		else if (IsViableFactoryClass(Factory, UMaterialInstanceConstant::StaticClass()))
		{
			MaterialConstantFactory = Factory;
		}
	}
}

bool FEditorBoidDataManager::IsViableFactoryClass(const UFactory* Factory, const UClass* ClassToCreate)
{
	return Factory->SupportedClass == ClassToCreate || ClassToCreate->IsChildOf(Factory->SupportedClass);
}
