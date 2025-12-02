// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimulationBoundsVisualizer.generated.h"


#define ENSURE_MESH_COMPONENT()\
	ENSURE_ALWAYS_MESSAGE_RETURN(IsValid(MeshComponent), "No valid mesh component.")


/**
 * Visual representation of simulation area containing mesh.
 */
UCLASS()
class BOIDS_PROJECT_EDITOR_API ASimulationBoundsVisualizer : public AActor
{
	GENERATED_BODY()

public:
	
	/** Default constructor. Creates actor's components. */
	ASimulationBoundsVisualizer();
	
	/** Subscribes to BoidDataEditorSubsystem for visualization changes events. */
	virtual void PostInitProperties() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Static mesh representing Bounds. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

private:
	
	UFUNCTION()
	void HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent);
	
	UFUNCTION()
	void HandleBoundsVisibilityChanged(bool bNewVisibility);
	
	/**
	 * Updates bounds size.
	 * @param NewBoundsExtent Extent of the Bounds mesh.
	 */
	void UpdateMeshBounds(const FVector& NewBoundsExtent);
	
	/** Conversion factor from Unreal units to meters, since mesh scale is defined in meters. */
	float BoundsMeshScaleFactor = 0.01f;
};
