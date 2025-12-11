
#include "Core/Managers/EditorBoidDataManager.h"
#include "AssetToolsModule.h"
#include "EditorAssetLibrary.h"
#include "FileHelpers.h"
#include "GameplayTagsManager.h"
#include "IAssetTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DataAssets/BoidsData.h"
#include "DataAssets/BoundsData.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"


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
	
	LoadedBoundsDataAsset->OverwriteData(CollisionBoundsData, CalculatedCollisionData);
	SaveAsset(LoadedBoundsDataAsset);
}

void FEditorBoidDataManager::SaveBoidsData(const FBoidsPlainInfo& BoidsDataToSave)
{
	UBoidsData* BoidsAssetToSave = LoadedBoidsDataAssets[BoidsDataToSave.Type];
	ENSURE_BOIDS_DATA_ASSET(IsValid(BoidsAssetToSave))
	
	BoidsAssetToSave->OverwriteData(BoidsDataToSave);
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
	EnsureBoidSpeciesDataAssets();	
	EnsureBoundsDataAssets();
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
	
	IAssetRegistry& AssetRegistry  = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	TArray<FAssetData> BoundsDataAssets;
	
	FARFilter Filter = GetFilterByClassAndPath(UBoundsData::StaticClass(), GetBoidsBoundsDirectoryName());
	AssetRegistry.GetAssets(Filter, BoundsDataAssets);
	
	if (BoundsDataAssets.Num() == 0)
	{
		CreateBoundsData();
	}
	else
	{
		LoadedBoundsDataAsset = Cast<UBoundsData>(BoundsDataAssets[0].GetAsset());
	}
}

void FEditorBoidDataManager::EnsureBoidsDataDirectoryExist() const
{
	if (!UEditorAssetLibrary::DoesDirectoryExist(GetBoidsSpeciesDataDirectoryName()))
	{
		UEditorAssetLibrary::MakeDirectory(GetBoidsSpeciesDataDirectoryName());
	}
}

void FEditorBoidDataManager::EnsureBoundsDataDirectoryExist() const
{
	if (!UEditorAssetLibrary::DoesDirectoryExist(GetBoidsBoundsDirectoryName()))
	{
		UEditorAssetLibrary::MakeDirectory(GetBoidsBoundsDirectoryName());
	}
}

FString FEditorBoidDataManager::GetBoidsSpeciesDataDirectoryName() const
{
	return "/" + UEditorAssetLibrary::GetProjectRootAssetDirectory() + BOIDS_SPECIES_DATA_DIRECTORY;
}

FString FEditorBoidDataManager::GetBoidsBoundsDirectoryName() const
{
	return "/" + UEditorAssetLibrary::GetProjectRootAssetDirectory() + BOUNDS_DATA_DIRECTORY;
}

void FEditorBoidDataManager::FilterAlreadyCreatedBoidDataAssets(TArray<FGameplayTag>& BoidSpeciesTags)
{
	IAssetRegistry& AssetRegistry  = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	TArray<FAssetData> BoidSpeciesDataAssets;
	
	FARFilter Filter = GetFilterByClassAndPath(UBoidsData::StaticClass(), GetBoidsSpeciesDataDirectoryName());
	
	AssetRegistry.GetAssets(Filter, BoidSpeciesDataAssets);
	
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

FARFilter FEditorBoidDataManager::GetFilterByClassAndPath(const UClass* ClassToFilter, const FString& PathToFilter) const
{
	FARFilter Filter;
	Filter.bRecursiveClasses = false;
	Filter.bRecursivePaths = false;
	Filter.ClassPaths.Add(ClassToFilter->GetClassPathName());
	Filter.PackagePaths.Add(FName(*PathToFilter));
	
	return Filter;
}

TArray<FGameplayTag> FEditorBoidDataManager::GetBoidsSpeciesTags() const
{
	const UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
	FGameplayTag FoundTag = GameplayTagsManager.RequestGameplayTag(FName(BOIDS_SPECIES_TAG));
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
	EnsureDataFactoriesExist();
	
	UObject* CreatedAsset = CreateAsset(AssetName, GetBoidsSpeciesDataDirectoryName(), UBoidsData::StaticClass(), 
		DataAssetFactory);
	return Cast<UBoidsData>(CreatedAsset);
}

void FEditorBoidDataManager::CreateBoundsData()
{
	EnsureDataFactoriesExist();
	
	const FString AssetDirectory = GetBoidsBoundsDirectoryName();
		
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
	if (IsValid(DataAssetFactory))
	{
		return;
	}
	
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	for (UFactory* Factory: AssetTools.GetNewAssetFactories())
	{
		if (IsViableFactoryClass(Factory, UDataAsset::StaticClass()))
		{
			DataAssetFactory = Factory;
			break;
		}
	}
}

bool FEditorBoidDataManager::IsViableFactoryClass(const UFactory* Factory, const UClass* ClassToCreate)
{
	return Factory->SupportedClass == ClassToCreate || ClassToCreate->IsChildOf(Factory->SupportedClass);
}
