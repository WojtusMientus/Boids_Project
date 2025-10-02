// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SelectableBoid.h"
#include "Boids_Project/Globals.h"
#include "VisualBoid.generated.h"

class UBoidManagerSubsystem;
class USceneComponent;
class UStaticMeshComponent;
class UArrowComponent;

/**
 * Visual representation of a Boid containing mesh and arrow component for debug purposes.
 * This actor is passive - the AVisualBoidManager class is responsible for updating its location and rotation every frame.
 */
UCLASS()
class BOIDS_PROJECT_API AVisualBoid : public AActor, public ISelectableBoid
{
	GENERATED_BODY()
	
public:

	/** Default constructor. Creates actor's components. */
	AVisualBoid();

	/**
	 * Updates Boid world position and rotation.
	 * @param NewPosition World position for the Boid.
	 * @param NewVelocity Velocity vector used to align the Boid's rotation.
	 */
	void UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity);

	/**
	 * Assigns a unique Boid ID.
	 * @param ID Unique ID for this Boid.
	 */
	void SetBoidID(int32 ID);

	//~ Begin ISelectableBoid Interface
	virtual FBoidSelection GetBoidSelection() const override;
	//~ End ISelectableBoid Interface

protected:

	/** Root scene component. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	/** The visual representation of the Boid. Collision disabled.  */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BoidMeshComponent;

	/** Arrow component indicating the forward vector (for debug purposes only). */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;

private:
	/**
	 * Rotates the actor to align with given velocity.
	 * @param Velocity Direction the Boid should be facing.
	 */
	void AlignRotationToVelocity(const FVector& Velocity);

	/** Unique Boid's ID used for selection and additional info. Set only during creation. */
	int32 BoidID = -1;

	/** Species type of the Boid. @note Currently unused - Setup for future development. */
	EBoidType BoidType;
};
