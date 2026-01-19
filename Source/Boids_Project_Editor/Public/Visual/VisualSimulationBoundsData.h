// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/SimulationPlainInfoData/BoundsPlainInfoData.h"
#include "GameFramework/Actor.h"
#include "VisualSimulationBoundsData.generated.h"

struct FCollisionData;
struct FVisualizerVisibility;
class UStaticMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;


/**
 * Visual representation of voxelized simulation area.
 * Shows voxelized wall data, its repelling forces with instanced static mesh.
 * Additionally, it shows bounds mesh and Flood Fill algorithm start point.
 */
UCLASS()
class BOIDS_PROJECT_EDITOR_API AVisualSimulationBoundsData : public AActor
{
	GENERATED_BODY()

public:
	
	/** Default constructor. Creates actor's components. */
	AVisualSimulationBoundsData();
	
	//~ Being AActor Interface

	/** Binds to BoidDataEditorSubsystem to necessary events. */
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	
	/** Unbinds from BoidDataEditorSubsystem if it was binded in the first place. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
	//~ End AActor Interface

protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	/** Static mesh representing simulation bounds. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> SimulationBoundsMeshComponent;
	
	/** Instanced static mesh representing voxelized wall data of simulation area. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> WallDataInstancedStaticMeshComponent;

	/** Instanced static mesh representing voxelized collision data of simulation area. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> CollisionForcesDataInstancedStaticMeshComponent;

	/** Static mesh representing voxel in simulation area (not in any mesh) for proper collision generation. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> SimulationAreaVoxelMeshComponent;
	
	
private:
	
	void CreateSceneRootComponent();
	void CreateSimulationBoundsMeshComponent();
	void CreateWallDataInstancedStaticMeshComponent();
	void CreateCollisionForcesDataInstancedStaticMeshComponent();
	void CreateSimulationAreaVoxelMeshComponent();
	
	void TrySubscribeToSubsystemEvent();
	void TryToUnsubscribeFromSubsystemEvent();
	
	UFUNCTION()
	void HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent);
	
	UFUNCTION()
	void HandleRegenerationCollisionData(const FCollisionData& CollisionData);
	
	UFUNCTION()
	void HandleSimulationAreaVoxelChanged(const FCollisionBoundsPlainInfo& CollisionBoundsPlainInfo);
	
	UFUNCTION()
	void HandleAnyComponentVisibilityChanged(const FVisualizerVisibility VisualizerVisibility);
	
	
	FVector GetSimulationVoxelStartPosition(const FSimulationBoundsPlainInfo& BoundsPlainData, 
		const FIntVector& StartingIndices) const;
	
	
	FDelegateHandle OnBoundsChangedHandle;
	FDelegateHandle OnCollisionRegenerationHandle;
	FDelegateHandle OnAnyVisibilityChangedHandle;
	FDelegateHandle OnSimulationAreaVoxelChangedHandle;
	
	/** Conversion factor from Unreal units to meters, since mesh scale is defined in meters. */
	const float MeshScaleFactor = 0.01f;

	/** Flag preventing multiple bindings to the same event. */
	bool bDidSubscribeToSubsystem = false;	
};
