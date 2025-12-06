// Copyright WojtusMientus

#pragma once

#include "GameplayTagContainer.h"
#include "Utilities/Macros/DebugMacros.h"

struct FBoundsPlainData;
struct FCollisionBoundsPlainData;
struct FBoidsPlainData;
class UBoundsData;
class UBoidsData;


#define ENSURE_BOUNDS_DATA_ASSET()\
	ENSURE_ALWAYS_MESSAGE_RETURN(IsValid(LoadedBoundsDataAsset), "Couldn't find loaded Bounds Data.")

#define ENSURE_BOIDS_DATA_ASSET(Expression)\
	ENSURE_ALWAYS_MESSAGE_RETURN(Expression, "Couldn't find loaded Boids Data.")
	
#define ENSURE_BOUNDS_DATA_ASSET_RETURN_VALUE()\
	ENSURE_ALWAYS_MESSAGE_RETURN_VALUE(IsValid(LoadedBoundsDataAsset), "Couldn't find loaded Bounds Data."\
			, FCollisionBoundsPlainData())

#define ENSURE_BOIDS_DATA_ASSET_CONTINUE(Expression)\
	ENSURE_ALWAYS_MESSAGE_CONTINUE(Expression, "Couldn't find loaded Boids Data.")


/*
 * Manager responsible for loading data to BoidEditorUtilityWidget and saving them.
 * Ensures that data exists before loading. 
 */
class FEditorBoidDataManager
{
	
public:
	
	void InitializeBoidSimulationData(FCollisionBoundsPlainData& OutBoundsData, TMap<FGameplayTag, 
		FBoidsPlainData>& OutBoidsData);
	
	void SaveBoundsData(const FCollisionBoundsPlainData& BoundsDataToSave, 
		const TArray<FVector>& CalculatedCollisionData);
	void SaveBoidsData(const FBoidsPlainData& BoidsDataToSave);
	void SaveAllBoidsData(const TMap<FGameplayTag, FBoidsPlainData>& AllBoidsDataToSave);
	
private:
	
	void EnsureNecessarySimulationData();
	
	FCollisionBoundsPlainData GetCopyOfBoundsData() const;
	TMap<FGameplayTag, FBoidsPlainData> GetCopyOfBoidsData() const;
	
	void EnsureBoidSpeciesDataAssets();
	void EnsureBoundsDataAssets();
	
	void EnsureBoidsDataDirectoryExist() const;
	void EnsureBoundsDataDirectoryExist() const;
	
	FString GetBoidsSpeciesDataDirectoryName() const;
	FString GetBoidsBoundsDirectoryName() const;
	
	/** Filters already created BoidData assets for certain species. */
	void FilterAlreadyCreatedBoidDataAssets(TArray<FGameplayTag>& OutRemainingBoidSpeciesTags);
	
	/** Creates filter for certain class that exist under certain directory. */
	FARFilter GetFilterByClassAndPath(const UClass* ClassToFilter, const FString& PathToFilter) const;
	
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
	
	const FString BOIDS_SPECIES_TAG = TEXT("Boid.Species");
	const FString BOIDS_SPECIES_DATA_DIRECTORY = TEXT("/DataAssets/Boids/Species");
	const FString BOUNDS_DATA_DIRECTORY = TEXT("/DataAssets/Boids/Bounds");
	
	const FString BOIDS_SPECIES_DATA_NAME_PREFIX = TEXT("DA_BoidData_");
	const FString BOUNDS_DATA_NAME = TEXT("DA_BoundsData");
};
