// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Core/Subsystems/BaseClass/GameInstanceSubsystemBase.h"
#include "Materials/MaterialInstanceConstant.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "VoxelGrids/VoxelGridData/VoxelGridData.h"
#include "GameplayTagContainer.h"
#include "BoidDataManagerSubsystem.generated.h"

class URuntimeDataLoaderSubsystem;
struct FLoadRequest;
struct FLoadedGroup;


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnSimulationDataLoadedEvent, const TArray<FBoidsSpeciesPlainInfo>& BoidsInfo, 
	const FEnvironmentCollisionVoxelGridData& EnvironmentVoxelGridData, 
	const FBoidCollisionVoxelGridData& BoidCollisionVoxelGridData)
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
	FGameplayTag RequestMappedKey(const int32 SpeciesIndex);
	void LoadSimulationDataAssets();
	void RequestMaterialInstanceAsset();
	
	FLinearColor GetSpeciesColor(int32 SpeciesIndex);
	
private:
	
	TArray<FLoadRequest> CreateBoidAndBoundsLoadRequest() const;
	TArray<FLoadRequest> CreateBoidMaterialLoadRequest() const;
	
	void HandleLoadedBoidsAndBoundsAssets(const TArray<FLoadedGroup>& LoadedAssets);
	void HandleLoadedMaterialAsset(const TArray<FLoadedGroup>& LoadedAssets);
	
	void HandleLoadedBoidsData(TArray<UObject*> LoadedBoidsAssets);
	void HandleLoadedBoundsData(TArray<UObject*> LoadedBoundsAsset, 
		FEnvironmentCollisionVoxelGridData& OutEnvironmentCollisionVoxelGridData,
		FBoidCollisionVoxelGridData& OutBoidCollisionVoxelGridData);
	
	
	UPROPERTY()
	TWeakObjectPtr<URuntimeDataLoaderSubsystem> RuntimeDataLoader;
	
	UPROPERTY()
	/** Global map of boids species and its index. */
	TMap<FGameplayTag, int32> BoidSpeciesIndexMap;
	
	UPROPERTY()
	TArray<FBoidsSpeciesPlainInfo> LoadedBoidsPlainInfos;
};
