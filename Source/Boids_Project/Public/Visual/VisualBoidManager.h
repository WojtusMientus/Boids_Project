
// Manager for Visual Boids in demo world.
// Listens to BoidManager events to update visual representation at runtime.
//
// Future: Migrate logic to Subsystem

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boids_Project/Globals.h"
#include "VisualBoidManager.generated.h"

class UBoidManagerSubsystem;
class AVisualBoid;

UCLASS()
class BOIDS_PROJECT_API AVisualBoidManager : public AActor
{
	GENERATED_BODY()
	
public:
	
	// ----- Constructor -----
	AVisualBoidManager();

protected:

	// ----- AActor Overrides -----
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVisualBoid> VisualBoidClass;

private:

	// ----- Private Helpers -----
	void InitializeBoids();

	UFUNCTION()
	void HandleBoidsUpdate();
	UFUNCTION()
	void HandleBoidsNumberUpdate(EBoidType BoidType, int32 NewBoidNumber);
	UFUNCTION()
	void HandleBoidsColorUpdate(EBoidType BoidType, FColor NewBoidColor);

	
	UPROPERTY(VisibleInstanceOnly)
	TArray<TObjectPtr<AVisualBoid>> VisualBoids;
	
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
};

