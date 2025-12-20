// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RuntimeDataLoaderSubsystem.generated.h"


struct FStreamableHandle;
struct FLoadedGroup;

DECLARE_DELEGATE_OneParam(FOnBatchLoadCompleteEvent, const TArray<FLoadedGroup>& LoadedAssets);

/** Helper enum for determining, how many assets to load. */
enum class EAssetRequestType: uint8
{
	SingleAsset,
	BatchAssets
};

struct FLoadRequest
{
	EAssetRequestType AssetRequestType = EAssetRequestType::SingleAsset;
	
	/** Directory path to search for assets. */
	FString AssetPath = TEXT("");
	
	/** Class filter. */
	TSubclassOf<UObject> AssetClass = nullptr;
};

/** Helper struct for grouping loaded assets. */
USTRUCT()
struct FLoadedGroup
{
	GENERATED_BODY()
	
	/** Class of grouped assets. */
	UPROPERTY()
	UClass* AssetClass = nullptr;
	
	UPROPERTY()
	TArray<UObject*> LoadedAssets = TArray<UObject*>();
};


/**
 * Generic async runtime asset loader. 
 */
UCLASS()
class BOIDS_PROJECT_API URuntimeDataLoaderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	//~ Being UGameInstanceSubsystem Interface
	virtual void Deinitialize() override;
	//~ End UGameInstanceSubsystem Interface
	
public:
	
	/** Loads async requested assets and broadcasts event OnLoadCompletion. */
	void LoadAssets(const TArray<FLoadRequest>& LoadRequests, FOnBatchLoadCompleteEvent OnBatchCompleteEventCallback);
	
	static TArray<FLoadRequest> CreateLoadRequest(EAssetRequestType RequestType, FString AssetPath, 
		TSubclassOf<UObject> AssetClass);
	
private:
	
	/** Retrieves SoftObjectPaths from requested assets. */
	void HandleLoadRequest(const FLoadRequest& LoadRequest, TArray<FSoftObjectPath>& OutAssetPathsToLoad);
	
	/** Called on asset loading completion. Groups final assets and broadcasts event OnBatchLoadCompleteEvent. */
	void OnBatchLoadFinished(TSharedPtr<FStreamableHandle> StreamableHandle, 
		FOnBatchLoadCompleteEvent OnBatchCompleteEventCallback);
	
	/** Groups loaded assets.  */
	TMap<UClass*, TArray<UObject*>> GroupLoadedAssetsByClass(TArray<UObject*> RawLoadedObjects);
	TArray<FLoadedGroup> ConvertAssetsToLoadedGroups(TMap<UClass*, TArray<UObject*>> GroupedAssets);
	
	/** Keeps handles alive to prevent garbage collection. */
	TArray<TSharedPtr<FStreamableHandle>> ActiveHandles;
};
