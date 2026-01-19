// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualBoid.generated.h"

class UBoidManagerSubsystem;
class USceneComponent;
class UStaticMeshComponent;
class UArrowComponent;


/**
 * Visual representation of a Boid containing mesh.
 * This actor is passive - the UVisualBoidManagerSubsystem is responsible for updating its location and rotation every frame.
 */
UCLASS()
class BOIDS_PROJECT_API AVisualBoid : public AActor
{
	GENERATED_BODY()
	
public:

	/** Default constructor. Creates actor's components. */
	AVisualBoid();
	
	void Initialize(const FVector& InitPosition = FVector::ZeroVector, 
		const FVector& InitVelocity = FVector::ZeroVector, UMaterialInterface* InitMaterial = nullptr);
	void Reset();
	
	
	/**
	 * Updates Boid world position and rotation.
	 * @param NewPosition World position for the Boid.
	 * @param NewVelocity Velocity vector used to align the Boid's rotation.
	 */
	void UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity);

	void SetMaterial(UMaterialInterface* NewMaterial);
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	/** The visual representation of the Boid. Collision disabled.  */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BoidMeshComponent;

private:
	
	void CreateRootSceneComponent();
	void CreateBoidMeshComponent();
	
	
	/**
	 * Rotates the actor to align with given velocity.
	 * @param Velocity Direction the Boid should be facing.
	 */
	void AlignRotationToVelocity(const FVector& Velocity);
};
