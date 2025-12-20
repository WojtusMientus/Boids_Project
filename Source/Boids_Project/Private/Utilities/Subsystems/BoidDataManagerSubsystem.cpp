
#include "Utilities/Subsystems/BoidDataManagerSubsystem.h"
#include "DataAssets/BoidsData.h"
#include "DataAssets/BoundsData.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "Utilities/BoidConstants.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Utilities/Subsystems/RuntimeDataLoaderSubsystem.h"


void UBoidDataManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World))
	ENSURE_ALWAYS_RETURN(World->IsGameWorld())
	
	Super::Initialize(Collection);
}

int32 UBoidDataManagerSubsystem::RequestMappedIndex(const FGameplayTag GameplayTag)
{
	return BoidSpeciesIndexMap.FindChecked(GameplayTag); 
}

void UBoidDataManagerSubsystem::LoadSimulationDataAssets()
{
	TArray<FLoadRequest> LoadRequests = CreateBoidAndBoundsLoadRequest();
	
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World))
	
	UGameInstance* GameInstance = World->GetGameInstance();
	ENSURE_ALWAYS_RETURN(IsValid(GameInstance))
	
	URuntimeDataLoaderSubsystem* RuntimeDataLoader = GameInstance->GetSubsystem<URuntimeDataLoaderSubsystem>();
	ENSURE_ALWAYS_RETURN(RuntimeDataLoader)
	
	RuntimeDataLoader->LoadAssets(LoadRequests, FOnBatchLoadCompleteEvent::CreateUObject(this, 
		&UBoidDataManagerSubsystem::HandleLoadedBoidsAndBoundsAssets));
}

TArray<FLoadRequest> UBoidDataManagerSubsystem::CreateBoidAndBoundsLoadRequest()
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

void UBoidDataManagerSubsystem::HandleLoadedBoidsAndBoundsAssets(const TArray<FLoadedGroup>& LoadedAssets)
{
	TArray<FBoidsPlainInfo> BoidsPlainInfoArray;
	FEnvironmentCollisionVoxelGridData VoxelGridData;
	
	for (FLoadedGroup LoadedGroup: LoadedAssets)
	{
		if (LoadedGroup.AssetClass == UBoidsData::StaticClass())
		{
			HandleLoadedBoidsData(LoadedGroup.LoadedAssets, BoidsPlainInfoArray);
		}
		else if (LoadedGroup.AssetClass == UBoundsData::StaticClass())
		{
			HandleLoadedBoundsData(LoadedGroup.LoadedAssets, VoxelGridData);
		}
	}
	
	for (const FBoidsPlainInfo BoidPlainInfo: BoidsPlainInfoArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loaded Boids Type: %s"), *BoidPlainInfo.Type.ToString())
	}
	
	for (auto Value: BoidSpeciesIndexMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boids Key Type: %s"), *Value.Key.ToString())
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Loaded Bounds Collision Multiplier: %f"), VoxelGridData.BoundsCollisionMultiplier);
	
	OnSimulationDataLoaded.Broadcast(BoidsPlainInfoArray, VoxelGridData);
}

void UBoidDataManagerSubsystem::HandleLoadedBoidsData(TArray<UObject*> LoadedBoidsAssets, 
	TArray<FBoidsPlainInfo>& OutBoidsInfoArray)
{
	if (LoadedBoidsAssets.Num() == 0)
	{
		return;
	}
	
	for (int i = 0; i < LoadedBoidsAssets.Num(); i++)
	{
		const UBoidsData* BoidsData = Cast<UBoidsData>(LoadedBoidsAssets[i]);
		ENSURE_ALWAYS_CONTINUE(IsValid(BoidsData))
		OutBoidsInfoArray.Add(FBoidsPlainInfo(BoidsData));
		BoidSpeciesIndexMap.FindOrAdd(BoidsData->Type, i);
	}	
}

void UBoidDataManagerSubsystem::HandleLoadedBoundsData(TArray<UObject*> LoadedBoundsAsset, 
	FEnvironmentCollisionVoxelGridData& OutVoxelGridData)
{
	if (LoadedBoundsAsset.Num() == 0)
	{
		return;
	}
	
	const UBoundsData* LoadedBounds = Cast<UBoundsData>(LoadedBoundsAsset[0]);
	OutVoxelGridData.OverwriteData(LoadedBounds);
}
