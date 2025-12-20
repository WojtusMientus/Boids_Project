
#include "Utilities//Subsystems/RuntimeDataLoaderSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Utilities/Libraries/RuntimeAssetUtils.h"


void URuntimeDataLoaderSubsystem::Deinitialize()
{
	Super::Deinitialize();
	ActiveHandles.Empty();
}

void URuntimeDataLoaderSubsystem::LoadAssets(const TArray<FLoadRequest>& LoadRequests,
                                             FOnBatchLoadCompleteEvent OnBatchCompleteEvent)
{
	TArray<FSoftObjectPath> AssetPathsToLoad;
	for (const FLoadRequest& LoadRequest : LoadRequests)
	{
		HandleLoadRequest(LoadRequest, AssetPathsToLoad);
	}
	
	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
	TSharedPtr<FStreamableHandle> NewLoadingHandle = StreamableManager.RequestAsyncLoad(AssetPathsToLoad);
	
	if (!NewLoadingHandle.IsValid())
	{
		OnBatchCompleteEvent.ExecuteIfBound(TArray<FLoadedGroup>());
		return;
	}
	
	ActiveHandles.Add(NewLoadingHandle);
	
	FStreamableDelegate OnBatchComplete = FStreamableDelegate::CreateUObject(this, 
		&URuntimeDataLoaderSubsystem::OnBatchLoadFinished, NewLoadingHandle, OnBatchCompleteEvent);
	
	NewLoadingHandle->BindCompleteDelegate(OnBatchComplete);
	
	if (NewLoadingHandle->HasLoadCompleted())
	{
		OnBatchLoadFinished(NewLoadingHandle, OnBatchCompleteEvent);
	}
}

TArray<FLoadRequest> URuntimeDataLoaderSubsystem::CreateLoadRequest(EAssetRequestType RequestType, FString AssetPath, 
		TSubclassOf<UObject> AssetClass)
{
	TArray<FLoadRequest> LoadRequests;
	LoadRequests.Add({RequestType, AssetPath, AssetClass});
	return LoadRequests;
}

void URuntimeDataLoaderSubsystem::HandleLoadRequest(const FLoadRequest& LoadRequest,
                                                    TArray<FSoftObjectPath>& OutAssetPathsToLoad)
{
	if (LoadRequest.AssetPath.IsEmpty() || !IsValid(LoadRequest.AssetClass))
	{
		return;
	}
		
	TArray<FAssetData> LoadedAssetData = FRuntimeAssetUtils::GetFilteredAssetDataByClassAndPath(LoadRequest.AssetClass,
		LoadRequest.AssetPath, false);
		
	if (LoadedAssetData.Num() == 0)
	{
		return;
	}
		
	if (LoadRequest.AssetRequestType == EAssetRequestType::SingleAsset)
	{
		OutAssetPathsToLoad.Add(LoadedAssetData[0].GetSoftObjectPath());
	}
	else if (LoadRequest.AssetRequestType == EAssetRequestType::BatchAssets)
	{
		for (const FAssetData& AssetData : LoadedAssetData)
		{
			OutAssetPathsToLoad.Add(AssetData.GetSoftObjectPath());
		}
	}
}

void URuntimeDataLoaderSubsystem::OnBatchLoadFinished(TSharedPtr<FStreamableHandle> StreamableHandle,
                                                      FOnBatchLoadCompleteEvent OnBatchCompleteEventCallback)
{
	ActiveHandles.Remove(StreamableHandle);
	
	if (!StreamableHandle.IsValid())
	{
		OnBatchCompleteEventCallback.ExecuteIfBound(TArray<FLoadedGroup>());
		return;
	}
	
	TArray<UObject*> RawLoadedObjects;
	StreamableHandle->GetLoadedAssets(RawLoadedObjects);
	
	TMap<UClass*, TArray<UObject*>> TemporaryGroupedAssets = GroupLoadedAssetsByClass(RawLoadedObjects);
	TArray<FLoadedGroup> GroupedAssets = ConvertAssetsToLoadedGroups(TemporaryGroupedAssets);
	
	OnBatchCompleteEventCallback.ExecuteIfBound(GroupedAssets);
}

TMap<UClass*, TArray<UObject*>> URuntimeDataLoaderSubsystem::GroupLoadedAssetsByClass(TArray<UObject*> RawLoadedObjects)
{
	TMap<UClass*, TArray<UObject*>> GroupedMap;
	
	for (UObject* LoadedObject : RawLoadedObjects)
	{
		if (LoadedObject)
		{
			GroupedMap.FindOrAdd(LoadedObject->GetClass()).Add(LoadedObject);
		}
	}
	
	return GroupedMap;
}

TArray<FLoadedGroup> URuntimeDataLoaderSubsystem::ConvertAssetsToLoadedGroups(
	TMap<UClass*, TArray<UObject*>> GroupedAssets)
{
	TArray<FLoadedGroup> LoadedGroups;
	
	for (const TPair<UClass*, TArray<UObject*>> CurrentPair : GroupedAssets)
	{
		FLoadedGroup CurrentLoadedGroup;
		CurrentLoadedGroup.AssetClass = CurrentPair.Key;
		CurrentLoadedGroup.LoadedAssets = CurrentPair.Value;
		LoadedGroups.Add(CurrentLoadedGroup);
	}
	
	return LoadedGroups;
}
