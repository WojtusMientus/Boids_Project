
// Visual representation of demo world bounds.
// Updates its mesh in response to BoidManager events.
// 
// Future: Support in-editor editing via external Boid Tool.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualWorldBounds.generated.h"

class UBoidManagerSubsystem;


UCLASS()
class BOIDS_PROJECT_API AVisualWorldBounds : public AActor
{
	GENERATED_BODY()
	
public:

	// ----- Constructor -----
	AVisualWorldBounds();

	// ----- AActor Overrides -----
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

private:

	// ----- Private Helpers -----
	UFUNCTION(BlueprintCallable)
	void HandleBoundsUpdate(const FVector& Center, const FVector& Extent);
	
	void UpdateMeshBounds(const FVector& NewBoundsExtent);

	
	float BoundsMeshScaleFactor = 0.01f;
};
