// Copyright WojtusMientus

#pragma once

#include "GameplayTagContainer.h"

struct FEnvironmentCollisionCellData;
struct FBoidsSpeciesPlainInfo;
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
		FBoidsSpeciesPlainInfo>& OutBoidsData);
	
	void SaveBoundsData(const FCollisionBoundsPlainInfo& BoundsDataToSave, 
		const TArray<FEnvironmentCollisionCellData>& CalculatedCollisionData);
	void SaveBoidsData(const FBoidsSpeciesPlainInfo& BoidsDataToSave);
	void SaveAllBoidsData(const TMap<FGameplayTag, FBoidsSpeciesPlainInfo>& AllBoidsDataToSave);
	
private:
	
	void EnsureNecessarySimulationData();
	
	FCollisionBoundsPlainInfo GetCopyOfBoundsData() const;
	TMap<FGameplayTag, FBoidsSpeciesPlainInfo> GetCopyOfBoidsData() const;
	
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
