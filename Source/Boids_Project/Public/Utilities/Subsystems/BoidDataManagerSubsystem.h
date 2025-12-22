// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Core/Subsystems/BaseClass/GameInstanceSubsystemBase.h"
#include "GameplayTagContainer.h"
#include "BoidDataManagerSubsystem.generated.h"


class URuntimeDataLoaderSubsystem;
struct FEnvironmentCollisionVoxelGridData;
struct FBoidsPlainInfo;
struct FLoadRequest;
struct FLoadedGroup;


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSimulationDataLoadedEvent, const TArray<FBoidsPlainInfo> BoidsInfo, 
	const FEnvironmentCollisionVoxelGridData VoxelGridData)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoidMaterialInstanceLoadedEvent, 
	UMaterialInstanceConstant* LoadedMaterialInstance)

/**
 * Data manager responsible for loading simulation data.
 * Stores a global map of Boids' type -> index for runtime parameters changes.
 */
UCLASS()
class BOIDS_PROJECT_API UBoidDataManagerSubsystem : public UGameInstanceSubsystemBase
{
	GENERATED_BODY()
	
public:
	
	FOnSimulationDataLoadedEvent OnSimulationDataLoaded;
	
	FOnBoidMaterialInstanceLoadedEvent OnBoidMaterialInstanceLoaded;
	
	
	//~ Being UGameInstanceSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End UGameInstanceSubsystem Interface
	
	int32 RequestMappedIndex(const FGameplayTag GameplayTag);
	void LoadSimulationDataAssets();
	void RequestMaterialInstanceAsset();
	
	FLinearColor GetSpeciesColor(int32 SpeciesIndex);
	
private:
	
	TArray<FLoadRequest> CreateBoidAndBoundsLoadRequest();
	TArray<FLoadRequest> CreateBoidMaterialLoadRequest();
	
	void HandleLoadedBoidsAndBoundsAssets(const TArray<FLoadedGroup>& LoadedAssets);
	void HandleLoadedMaterialAsset(const TArray<FLoadedGroup>& LoadedAssets);
	
	
	void HandleLoadedBoidsData(TArray<UObject*> LoadedBoidsAssets);
	void HandleLoadedBoundsData(TArray<UObject*> LoadedBoundsAsset, FEnvironmentCollisionVoxelGridData& OutVoxelGridData);
	
	
	UPROPERTY()
	TWeakObjectPtr<URuntimeDataLoaderSubsystem> RuntimeDataLoader;
	
	UPROPERTY()
	TMap<FGameplayTag, int32> BoidSpeciesIndexMap;
	
	UPROPERTY()
	TArray<FBoidsPlainInfo> LoadedBoidsPlainInfos;
};
