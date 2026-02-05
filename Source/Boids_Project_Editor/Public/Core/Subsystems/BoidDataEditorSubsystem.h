// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Core/Managers/EditorBoidDataManager.h"
#include "Core/Managers/CollisionDataGenerator.h"
#include "Visual/VisualizerVisibility.h"
#include "BoidDataEditorSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnCollisionDataVisualizationEvent, const FCollisionData& CollisionData)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSimulationBoundsChangedEvent, const FVector& NewCenter, 
	const FVector& NewExtent)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnySimulationBoundsDataVisibilityChangedEvent, 
	const FVisualizerVisibility NewVisibility)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSimulationAreaVoxelChangedEvent, 
	const FCollisionBoundsPlainInfo& CollisionBoundsPlainInfo)


/**
 * Subsystem for editor communication between tool and its editor only functionality like data saving and visualization.
 * Ensures necessary assets exist and generates collision data for later simulation use.
 * Handles communication between tool and editor-only actors.
 */
UCLASS()
class BOIDS_PROJECT_EDITOR_API UBoidDataEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	
	FOnCollisionDataVisualizationEvent OnCollisionDataVisualization;
	
	FOnSimulationBoundsChangedEvent OnSimulationBoundsChanged;
	
	FOnAnySimulationBoundsDataVisibilityChangedEvent OnAnySimulationBoundsDataVisibilityChanged;
	
	FOnSimulationAreaVoxelChangedEvent OnSimulationAreaVoxelChanged;
	
	
	//~ Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End USubsystem Interface
	
	UFUNCTION(BlueprintCallable)
	void InitializeNecessarySimulationData(FCollisionBoundsPlainInfo& BoundsData, 
		TMap<FGameplayTag, FBoidsSpeciesPlainInfo>& BoidsData);
	
	UFUNCTION(BlueprintCallable)
	void HandleInitialVisualizationEvent();
	
	UFUNCTION(BlueprintCallable)
	void RegenerateCollisionDataAndSave(const FCollisionBoundsPlainInfo& BoundsData);
	
	UFUNCTION(BlueprintCallable)
	void SaveBoidsData(const FBoidsSpeciesPlainInfo& BoidsData);
	UFUNCTION(BlueprintCallable)
	void SaveAllBoidsData(const TMap<FGameplayTag, FBoidsSpeciesPlainInfo>& AllBoidsData);
	
	UFUNCTION(BlueprintCallable)
	void HandleBoundsChanged(const FVector& NewCenter,	const FVector& NewExtent);
	
	UFUNCTION(BlueprintCallable)
	void HandleSimulationBoundsDataVisibilityChanged(const FVisualizerVisibility VisualizerVisibility);

	UFUNCTION(BlueprintCallable)
	void HandleSimulationAreaVoxelChanged(const FCollisionBoundsPlainInfo& CollisionBoundsPlainInfo);
	
	
private:
	
	/** Helper classes for data generation and saving. */
	TUniquePtr<FEditorBoidDataManager> BoidDataManager;
	TUniquePtr<FCollisionDataGenerator> CollisionDataGenerator;
};
