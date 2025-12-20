// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/GameInstanceSubsystemBase.h"
#include "VisualBoidManagerSubsystem.generated.h"


class URuntimeDataLoaderSubsystem;
class UBoidManagerSubsystem;
struct FLoadedGroup;
class AVisualBoid;
struct FGameplayTag;


/**
 * 
 */
UCLASS()
class BOIDS_PROJECT_API UVisualBoidManagerSubsystem : public UGameInstanceSubsystemBase
{
	GENERATED_BODY()
	
public:

	//~ Begin UGameInstanceSubsystemBase Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End UGameInstanceSubsystemBase Interface
	
private:

	void PostWorldInitialized(UWorld* World, FWorldInitializationValues WorldInitializationValues);
	
	void GetDataFromSimulationSettings();
	void BindNecessaryDelegates();
	
	/** Populates the VisualBoids array with the correct number of Boids. */
	void InitializeBoids();
	void InitializeBoid(AVisualBoid* BoidToInitialize, int32 BoidIndex);
	
	/** Asks for UMaterialInstanceConstant saved on the disk. */
	void RequestMaterialInstanceAsset();
	void HandleLoadedMaterialAsset(const TArray<FLoadedGroup>& LoadedAssets);
	
	
	/** Updates the world location and rotation of all visual Boids. */
	void HandleBoidsUpdate();
	
	
	void HandleBoidsNumberUpdate(FGameplayTag BoidType, int32 NewBoidCount);
	
#if WITH_EDITOR
	/** Handler function for UBoidEditorUtilityWidget color changes. */
	void HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor);
#endif
	
	
	UPROPERTY()
	TSubclassOf<AVisualBoid> VisualBoidClass;
	
	/** Array of all spawned visual Boid actors. */
	UPROPERTY()
	TArray<TObjectPtr<AVisualBoid>> VisualBoids;

	/** Weak reference to the BoidManagerSubsystem for event binding and unbinding. */
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
	
	TWeakObjectPtr<URuntimeDataLoaderSubsystem> RuntimeDataLoaderSubsystem;
	
	/** Created material for assigning certain color to Boids at runtime. */
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
#if WITH_EDITOR
	FDelegateHandle BoidsColorChangeDelegateHandle;
#endif
};
