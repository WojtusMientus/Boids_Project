
#include "Utilities/Subsystems/BoidDataManagerSubsystem.h"
#include "DataAssets/BoidsData.h"
#include "DataAssets/BoundsData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Utilities/BoidConstants.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Utilities/Subsystems/RuntimeDataLoaderSubsystem.h"


void UBoidDataManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World))
	ENSURE_ALWAYS_RETURN(World->IsGameWorld())
	
	UGameInstance* GameInstance = World->GetGameInstance();
	ENSURE_ALWAYS_RETURN(IsValid(GameInstance))
	
	RuntimeDataLoader = GameInstance->GetSubsystem<URuntimeDataLoaderSubsystem>();
	ENSURE_ALWAYS_RETURN(RuntimeDataLoader.IsValid())
	
	Super::Initialize(Collection);
}

int32 UBoidDataManagerSubsystem::RequestMappedIndex(const FGameplayTag GameplayTag)
{
	return BoidSpeciesIndexMap.FindChecked(GameplayTag); 
}

FGameplayTag UBoidDataManagerSubsystem::RequestMappedKey(const int32 SpeciesIndex)
{
	return *BoidSpeciesIndexMap.FindKey(SpeciesIndex);
}

void UBoidDataManagerSubsystem::LoadSimulationDataAssets()
{
	ENSURE_ALWAYS_RETURN(RuntimeDataLoader.IsValid())
	
	TArray<FLoadRequest> LoadRequests = CreateBoidAndBoundsLoadRequest();
	RuntimeDataLoader->LoadAssets(LoadRequests, FOnBatchLoadCompleteEvent::CreateUObject(this, 
		&UBoidDataManagerSubsystem::HandleLoadedBoidsAndBoundsAssets));
}

void UBoidDataManagerSubsystem::RequestMaterialInstanceAsset()
{
	ENSURE_ALWAYS_RETURN(RuntimeDataLoader.IsValid())
	
	TArray<FLoadRequest> LoadRequests = CreateBoidMaterialLoadRequest();
	RuntimeDataLoader->LoadAssets(LoadRequests, FOnBatchLoadCompleteEvent::CreateUObject(this, 
		&UBoidDataManagerSubsystem::HandleLoadedMaterialAsset));
}

FLinearColor UBoidDataManagerSubsystem::GetSpeciesColor(int32 SpeciesIndex)
{
	if (!LoadedBoidsPlainInfos.IsValidIndex(SpeciesIndex))
	{
		return FLinearColor::White;
	}
	
	return LoadedBoidsPlainInfos[SpeciesIndex].Color;
}

TArray<FLoadRequest> UBoidDataManagerSubsystem::CreateBoidAndBoundsLoadRequest() const
{
	TArray<FLoadRequest> LoadRequests;
	
	FLoadRequest BoidLoadRequest = FLoadRequest(EAssetRequestType::BatchAssets, FBoidConstants::Paths::SpeciesPath, 
		UBoidsData::StaticClass());
	FLoadRequest BoundsLoadRequest = FLoadRequest(EAssetRequestType::SingleAsset, FBoidConstants::Paths::BoundsPath, 
		UBoundsData::StaticClass());
	LoadRequests.Add(BoidLoadRequest);
	LoadRequests.Add(BoundsLoadRequest);
	
	return LoadRequests;
}

TArray<FLoadRequest> UBoidDataManagerSubsystem::CreateBoidMaterialLoadRequest() const 
{
	return URuntimeDataLoaderSubsystem::CreateLoadRequest(EAssetRequestType::SingleAsset, 
		FBoidConstants::Paths::MaterialsPath, UMaterialInstanceConstant::StaticClass());
}

void UBoidDataManagerSubsystem::HandleLoadedBoidsAndBoundsAssets(const TArray<FLoadedGroup>& LoadedAssets)
{
	FEnvironmentCollisionVoxelGridData EnvironmentCollisioVoxelGridData;
	FBoidCollisionVoxelGridData BoidCollisionVoxelGridData;
	
	for (FLoadedGroup LoadedGroup: LoadedAssets)
	{
		if (LoadedGroup.AssetClass == UBoidsData::StaticClass())
		{
			HandleLoadedBoidsData(LoadedGroup.LoadedAssets);
		}
		else if (LoadedGroup.AssetClass == UBoundsData::StaticClass())
		{
			HandleLoadedBoundsData(LoadedGroup.LoadedAssets, EnvironmentCollisioVoxelGridData, 
				BoidCollisionVoxelGridData);
		}
	}
	
	OnSimulationDataLoaded.Broadcast(LoadedBoidsPlainInfos, EnvironmentCollisioVoxelGridData, BoidCollisionVoxelGridData);
}

void UBoidDataManagerSubsystem::HandleLoadedMaterialAsset(const TArray<FLoadedGroup>& LoadedAssets)
{
	if (LoadedAssets.IsEmpty() || LoadedAssets[0].LoadedAssets.IsEmpty())
	{
		return;
	}
	
	UMaterialInstanceConstant* LoadedMaterialInstance = Cast<UMaterialInstanceConstant>(LoadedAssets[0].LoadedAssets[0]);
	ENSURE_ALWAYS_RETURN(IsValid(LoadedMaterialInstance))
	
	OnBoidMaterialInstanceLoaded.Broadcast(LoadedMaterialInstance);
}

void UBoidDataManagerSubsystem::HandleLoadedBoidsData(TArray<UObject*> LoadedBoidsAssets)
{
	if (LoadedBoidsAssets.Num() == 0)
	{
		return;
	}
	
	for (UObject* LoadedObject: LoadedBoidsAssets)
	{
		const UBoidsData* BoidsData = Cast<UBoidsData>(LoadedObject);
		ENSURE_ALWAYS_CONTINUE(IsValid(BoidsData))
		
		LoadedBoidsPlainInfos.Add(FBoidsSpeciesPlainInfo(BoidsData));
		BoidSpeciesIndexMap.FindOrAdd(BoidsData->Type, BoidSpeciesIndexMap.Num());
	}
}

void UBoidDataManagerSubsystem::HandleLoadedBoundsData(TArray<UObject*> LoadedBoundsAsset, 
	FEnvironmentCollisionVoxelGridData& OutEnvironmentCollisionVoxelGridData,
	FBoidCollisionVoxelGridData& OutBoidCollisionVoxelGridData)
{
	if (LoadedBoundsAsset.Num() == 0)
	{
		return;
	}
	
	const UBoundsData* LoadedBounds = Cast<UBoundsData>(LoadedBoundsAsset[0]);
	OutEnvironmentCollisionVoxelGridData.OverwriteData(LoadedBounds);
	OutBoidCollisionVoxelGridData.OverwriteData(LoadedBounds);
	OutBoidCollisionVoxelGridData.InitializeBoidCollisionArray();
}
