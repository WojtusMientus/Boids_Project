// Copyright WojtusMientus

#pragma once

#include "GameplayTagContainer.h"
#include "Utilities/Macros/DebugMacros.h"

struct FEnvironmentCollisionCellData;
struct FBoidsPlainInfo;
struct FCollisionBoundsPlainInfo;
class UBoundsData;
class UBoidsData;


#define ENSURE_BOUNDS_DATA_ASSET()\
	ENSURE_ALWAYS_MESSAGE_RETURN(IsValid(LoadedBoundsDataAsset), "Couldn't find loaded Bounds Data.")

#define ENSURE_BOIDS_DATA_ASSET(Expression)\
	ENSURE_ALWAYS_MESSAGE_RETURN(Expression, "Couldn't find loaded Boids Data.")
	
#define ENSURE_BOUNDS_DATA_ASSET_RETURN_VALUE()\
	ENSURE_ALWAYS_MESSAGE_RETURN_VALUE(IsValid(LoadedBoundsDataAsset), "Couldn't find loaded Bounds Data."\
			, FCollisionBoundsPlainInfo())

#define ENSURE_BOIDS_DATA_ASSET_CONTINUE(Expression)\
	ENSURE_ALWAYS_MESSAGE_CONTINUE(Expression, "Couldn't find loaded Boids Data.")


/*
 * Manager responsible for loading data to BoidEditorUtilityWidget and saving them.
 * Ensures that data exists before loading. 
 */
class FEditorBoidDataManager
{
	
public:
	
	void InitializeBoidSimulationData(FCollisionBoundsPlainInfo& OutBoundsData, TMap<FGameplayTag, 
		FBoidsPlainInfo>& OutBoidsData);
	
	void SaveBoundsData(const FCollisionBoundsPlainInfo& BoundsDataToSave, 
		const TArray<FEnvironmentCollisionCellData>& CalculatedCollisionData);
	void SaveBoidsData(const FBoidsPlainInfo& BoidsDataToSave);
	void SaveAllBoidsData(const TMap<FGameplayTag, FBoidsPlainInfo>& AllBoidsDataToSave);
	
private:
	
	void EnsureNecessarySimulationData();
	
	FCollisionBoundsPlainInfo GetCopyOfBoundsData() const;
	TMap<FGameplayTag, FBoidsPlainInfo> GetCopyOfBoidsData() const;
	
	void EnsureBoidSpeciesDataAssets();
	void EnsureBoundsDataAssets();
	void EnsureMaterialAssets();
	
	void EnsureBaseMaterialAsset();
	void EnsureMaterialInstanceAsset();
	
	void EnsureDirectoryExist(const FString& DirectoryName) const;
	void EnsureBoidsDataDirectoryExist() const;
	void EnsureBoundsDataDirectoryExist() const;
	void EnsureMaterialDataDirectoryExist() const;
	
	/** Filters already created BoidData assets for certain species. */
	void FilterAlreadyCreatedBoidDataAssets(TArray<FGameplayTag>& OutRemainingBoidSpeciesTags);
	
	/** Retrieves AssetData of certain class in certain directory. */
	TArray<FAssetData> GetFilteredAssetDataByClassAndPath(UClass* ClassToFilter, const FString& PathToFilter) const;
	
	/** Returns all Boid.Species tags. */
	TArray<FGameplayTag> GetBoidsSpeciesTags() const;
	
	void CreateBoidSpeciesDataWithTag(const FGameplayTag Tag);
	UBoidsData* CreateBoidsData(const FString& AssetName);
	void CreateBoundsData();
	UObject* CreateAsset(const FString& AssetName, const FString& AssetDirectory, UClass* AssetClass, 
		UFactory* AssetFactory);
	
	void SaveAsset(UObject* AssetToSave);
	
	void EnsureDataFactoriesExist();
	bool IsViableFactoryClass(const UFactory* Factory, const UClass* ClassToCreate);
	
	
	UBoundsData* LoadedBoundsDataAsset = nullptr;
	TMap<FGameplayTag, UBoidsData*> LoadedBoidsDataAssets;
	
	UFactory* DataAssetFactory = nullptr;
	UFactory* MaterialFactory = nullptr;
	UFactory* MaterialConstantFactory = nullptr;
	
	const FString BOIDS_SPECIES_DATA_NAME_PREFIX = TEXT("DA_BoidData_");
	const FString BOUNDS_DATA_NAME = TEXT("DA_BoundsData");
	
	const FString MATERIAL_DATA_NAME = TEXT("M_BoidsBase");
	const FString MATERIAL_INSTANCE_DATA_NAME = TEXT("MI_BoidsBase");
};
