// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/GameInstanceSubsystemBase.h"
#include "Utilities/Subsystems/BoidDataManagerSubsystem.h"
#include "VisualBoidManagerSubsystem.generated.h"


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

	void HandleVisualSimulationInitialization();
	
	void PostWorldInitialized(UWorld* World, FWorldInitializationValues WorldInitializationValues);
	
	void GetDataFromSimulationSettings();
	void BindNecessaryDelegates();
	
	/** Populates the VisualBoids array with the correct number of Boids. */
	void InitializeBoids();
	void InitializeBoid(AVisualBoid* BoidToInitialize, int32 BoidIndex);
	
	void InitializeBoidsTest();
	void InitializeBoidTest(AVisualBoid* BoidToInitialize, int32 SpeciesIndex, int32 BoidIndex);
	
	/** Asks for UMaterialInstanceConstant saved on the disk. */
	void RequestMaterialInstanceAsset();
	void HandleLoadedMaterialAsset(UMaterialInstanceConstant* LoadedMaterialInstance);
	
	
	/** Updates the world location and rotation of all visual Boids. */
	void HandleBoidsUpdate();
	
	void HandleBoidsUpdateTest();
	
	
	void HandleBoidsNumberUpdate(FGameplayTag BoidType, int32 NewBoidCount);
	
#if WITH_EDITOR
	/** Handler function for UBoidEditorUtilityWidget color changes. */
	void HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor);
#endif
	
	
	bool bIsVisualSimulationReadyToInitialize = false;
	bool bIsVisualSimulationInitialized = false;
	

	TArray<TArray<TObjectPtr<AVisualBoid>>> DifferentSpeciesBoids;
	
	UPROPERTY()
	TSubclassOf<AVisualBoid> VisualBoidClass;
	
	/** Array of all spawned visual Boid actors. */
	UPROPERTY()
	TArray<TObjectPtr<AVisualBoid>> VisualBoids;
	
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> BoidMaterials;
	

	/** Weak reference to the BoidManagerSubsystem for event binding and unbinding. */
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
	
	TWeakObjectPtr<UBoidDataManagerSubsystem> BoidDataManager;

	
#if WITH_EDITOR
	FDelegateHandle BoidsColorChangeDelegateHandle;
#endif
};
