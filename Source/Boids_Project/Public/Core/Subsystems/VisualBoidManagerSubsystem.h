// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/GameInstanceSubsystemBase.h"
#include "Utilities/Subsystems/BoidDataManagerSubsystem.h"
#include "VisualBoidManagerSubsystem.generated.h"

class UVisualBoidSpecies;
struct FLoadedGroup;
class AVisualBoid;
struct FGameplayTag;
class UBoidManagerSubsystem;


/**
 * Bridge between the boid simulation and its visual representation,
 * Updating actor transforms and materials in response to UBoidManagerSubsystem and UBoidEditorUtilityWidget events.
 */
UCLASS()
class BOIDS_PROJECT_API UVisualBoidManagerSubsystem : public UGameInstanceSubsystemBase
{
	GENERATED_BODY()
	
public:

	//~ Begin UGameInstanceSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End UGameInstanceSubsystem Interface
	
private:

	void BindNecessarySimulationDelegates();
	
	void HandleVisualSimulationInitialization();
	
	void InitializeAllBoidsSpecies();
	void InitializeBoidSpecies(const int SpeciesIndex, const TSubclassOf<AVisualBoid> VisualBoidClass);
	void InitializeVisualBoid(int32 SpeciesIndex, int32 BoidIndex);
	
	TSubclassOf<AVisualBoid> GetVisualBoidClass();
	
	/** Sends request to UBoidRuntimeDataManager for UMaterialInstanceConstant asset. */
	void RequestMaterialInstanceAsset();
	void HandleLoadedMaterialAssets(UMaterialInstanceConstant* LoadedMaterialInstance);
	void InitializeBoidMaterials(UMaterialInstanceConstant* LoadedMaterialInstance);
	
	/** Updates the world location and rotation of all visual Boids. */
	void HandleBoidsUpdate();
	
	
	UPROPERTY()
	TArray<TObjectPtr<UVisualBoidSpecies>> VisualBoids;

	/** Weak reference to the BoidManagerSubsystem for event binding and unbinding. */
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
	
	/** Weak reference to the BoidDataManager for getting converted gameplay tag to array index. */
	TWeakObjectPtr<UBoidDataManagerSubsystem> BoidDataManager;

	
#if WITH_EDITOR
	void SubscribeToGlobalEditorDelegates();
	void UnsubscribeFromGlobalEditorDelegates();
	
	/** Handler function for any boid number update from UBoidManagerSubsystem.  */
	void HandleBoidsNumberUpdate();
	void HandleBoidAddition(const int32 SpeciesID, const int32 CountToAdd);
	void BroadcastOnBoidsNumberUpdateFinish();
	
	/** Handler function for UBoidEditorUtilityWidget color changes. */
	void HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor);
	
	
	FDelegateHandle BoidsColorUpdateDelegateHandle;
	FDelegateHandle BoidsNumberUpdateDelegateHandle;
#endif
};
