// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualWorldBounds.generated.h"

class UBoidManagerSubsystem;

/**
 * Visual representation of Bounds containing mesh.
 */
UCLASS()
class BOIDS_PROJECT_API AVisualWorldBounds : public AActor
{
	GENERATED_BODY()
	
public:

	/** Default constructor. Creates actor's components. */
	AVisualWorldBounds();

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	/** Static mesh representing Bounds. Collision disabled. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

private:
	/**
	 * Updates Bounds world position and size.
	 * @param NewCenter World location of the Bounds.
	 * @param NewExtent Extent of the Bounds mesh.
	 */
	UFUNCTION(BlueprintCallable)
	void HandleBoundsUpdate(const FVector& NewCenter, const FVector& NewExtent);

	/**
	 * Updates bounds size.
	 * @param NewBoundsExtent Extent of the Bounds mesh.
	 */
	void UpdateMeshBounds(const FVector& NewBoundsExtent);

	/** Weak reference to the BoidManagerSubsystem for event binding and unbinding. */
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
	
	/** Conversion factor from Unreal units to meters, since mesh scale is defined in meters. */
	float BoundsMeshScaleFactor = 0.01f;
};
