// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Core/Managers/EditorBoidDataManager.h"
#include "Core/Managers/CollisionDataGenerator.h"
#include "Visual/VisualizerVisibility.h"
#include "BoidDataEditorSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnCollisionDataRegenerationEvent, const FCollisionData& /* VisualizerData */);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSimulationBoundsChangedEvent, const FVector& /* NewCenter */, 
	const FVector& /* NewExtent */);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnyVisibilityChangedEvent, const FVisualizerVisibility& /* NewVisibility */ );


#define ENSURE_BOIDS_DATA_MANAGER()\
	ENSURE_ALWAYS_MESSAGE_RETURN(BoidDataManager.IsValid(), "Boid Data Manager Not Valid.")

#define ENSURE_COLLISION_DATA_MANAGER()\
	ENSURE_ALWAYS_MESSAGE_RETURN(CollisionDataGenerator.IsValid(), "Collision Data Manager Not Valid.")


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
	
	FOnCollisionDataRegenerationEvent OnCollisionDataRegenerationEvent;
	
	FOnSimulationBoundsChangedEvent OnSimulationBoundsChangedEvent;
	
	FOnAnyVisibilityChangedEvent OnAnyVisibilityChanged;
	
	//~ Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End USubsystem Interface
	
	UFUNCTION(BlueprintCallable)
	void InitializeNecessarySimulationData(FCollisionBoundsPlainData& BoundsData, 
		TMap<FGameplayTag, FBoidsPlainData>& BoidsData);
	
	UFUNCTION(BlueprintCallable)
	void RegenerateCollisionDataAndSave(const FCollisionBoundsPlainData& BoundsData);
	
	UFUNCTION(BlueprintCallable)
	void SaveBoidsData(const FBoidsPlainData& BoidsData);
	UFUNCTION(BlueprintCallable)
	void SaveAllBoidsData(const TMap<FGameplayTag, FBoidsPlainData>& AllBoidsData);
	
	UFUNCTION(BlueprintCallable)
	void HandleBoundsChanged(const FVector& NewCenter,	const FVector& NewExtent);
	
	UFUNCTION(BlueprintCallable)
	void HandleBoundsVisibilityChanged(bool bIsVisible);
	
	UFUNCTION(BlueprintCallable)
	void HandleCollisionDataVisibilityChanged(bool bIsVisible);
	
private:
	
	/** Helper classes for data generation and saving. */
	TUniquePtr<FEditorBoidDataManager> BoidDataManager;
	TUniquePtr<FCollisionDataGenerator> CollisionDataGenerator;
};
