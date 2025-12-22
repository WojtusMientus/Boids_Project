// Copyright WojtusMientus

#pragma once

#include "GameplayTagContainer.h"
#include "Utilities/Macros/DebugMacros.h"

struct FEnvironmentCollisionCellData;
struct FBoidsPlainInfo;
struct FCollisionBoundsPlainInfo;
class UBoundsData;
class UBoidsData;


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
	
	void EnsureBoidsDataDirectoryExist() const;
	void EnsureBoundsDataDirectoryExist() const;
	void EnsureMaterialDataDirectoryExist() const;
	
	/** Filters already created BoidData assets for certain species. */
	void FilterAlreadyCreatedBoidDataAssets(TArray<FGameplayTag>& OutRemainingBoidSpeciesTags);
	
	/** Returns all Boid.Species tags. */
	TArray<FGameplayTag> GetBoidsSpeciesTags() const;
	
	void CreateBoidSpeciesDataWithTag(const FGameplayTag Tag);
	
	
	UBoundsData* LoadedBoundsDataAsset = nullptr;
	TMap<FGameplayTag, UBoidsData*> LoadedBoidsDataAssets;
	
	UMaterial* LoadedMaterialAsset = nullptr;
};
