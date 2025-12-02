// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utilities/Macros/DebugMacros.h"
#include "CollisionGridVisualizer.generated.h"

class UBoundsData;
struct FBoundsPlainData;
class UHierarchicalInstancedStaticMeshComponent;


#define ENSURE_INSTANCE_MESH_COMPONENT()\
	ENSURE_ALWAYS_MESSAGE_RETURN(IsValid(InstancedStaticMeshComponent), "No valid instanced mesh component.")


/**
 * Visual representation of voxelized simulation area.
 * Shows voxelized wall data and its repelling forces. 
 */
UCLASS()
class BOIDS_PROJECT_EDITOR_API ACollisionGridVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	
	/** Default constructor. Creates actor's components. */
	ACollisionGridVisualizer();
	
	/** Subscribes to BoidDataEditorSubsystem for visualization changes events. */
	virtual void PostInitProperties() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	/** Instanced static mesh representing voxelized simulation area. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> InstancedStaticMeshComponent;
	
private:
	
	UFUNCTION()
	void HandleRegenerationCollisionData(const TArray<bool>& CollisionData, const FBoundsPlainData& BoundsData);
	
	UFUNCTION()
	void HandleCollisionDataVisibilityChanged(bool bIsVisible);
	
	/** Conversion factor from Unreal units to meters, since mesh scale is defined in meters. */
	const float MeshScaleFactor = 0.01f;
};
