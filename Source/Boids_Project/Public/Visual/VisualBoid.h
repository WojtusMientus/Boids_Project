// Copyright WojtusMientus

// Visual representation of a Boid for demo purposes.
//
// Future: Support multiple boid species via BoidEnum for richer interactions.

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


UCLASS()
class BOIDS_PROJECT_API AVisualBoid : public AActor, public ISelectableBoid
{
	GENERATED_BODY()
	
public:

	// ----- Constructor -----
	AVisualBoid();

	// ----- Public API -----
	void UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity);
	void SetBoidID(int32 ID);

	virtual FBoidSelection GetBoidSelection() const override;
	
protected:

	// ----- Components -----
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BoidMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;

private:

	// ----- Private Helpers -----
	void AlignRotationToVelocity(const FVector& Velocity);
	
	int32 BoidID;
	EBoidType BoidType;
};
