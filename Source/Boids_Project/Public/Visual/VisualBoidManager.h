// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualBoidManager.generated.h"

class UBoidManagerSubsystem;
class AVisualBoid;

/**
 * Manager for the in-scene visual representation of Boids.
 * Listens to BoidManager events to update visual representation at runtime.
 */
// TODO: Move logic to a subsystem in future development.
UCLASS()
class BOIDS_PROJECT_API AVisualBoidManager : public AActor
{
	GENERATED_BODY()
	
public:
	
	/** Default constructor. */
	AVisualBoidManager();

protected:

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface

	/** Subclass of AVisualBoid to spawn at runtime. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVisualBoid> VisualBoidClass;

private:

	/** Populates the VisualBoid array with the correct number of Boids. Called only during object creation. */
	void InitializeBoids();

	/** Updates the world location and rotation of all visual Boids. */
	UFUNCTION()
	void HandleBoidsUpdate();

	/**
	 * Updates number of Boids.
	 * @param BoidType The type of Boid that was added or removed.
	 * @param NewBoidCount Number of Boids for that species.
	 */
	UFUNCTION()
	void HandleBoidsNumberUpdate(FGameplayTag BoidType, int32 NewBoidCount);
	// TODO: Currently unused - Setup for future development.

	/**
	 * Updates color of certain BoidType.
	 * @param BoidType The type of Boid whose color was changed.
	 * @param NewBoidColor Color for that species.
	 */
	UFUNCTION()
	void HandleBoidsColorUpdate(FGameplayTag BoidType, FColor NewBoidColor);
	// TODO: Currently unused - Setup for future development.
	
	/** Array of all spawned visual Boid actors. */
	UPROPERTY(VisibleInstanceOnly)
	TArray<TObjectPtr<AVisualBoid>> VisualBoids;

	/** Weak reference to the BoidManagerSubsystem for event binding and unbinding. */
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
};

