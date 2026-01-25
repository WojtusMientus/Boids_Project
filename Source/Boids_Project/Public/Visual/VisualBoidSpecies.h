// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualBoidSpecies.generated.h"

class UInstancedStaticMeshComponent;


/**
 *  Runtime container for visual boids of a single species.
 *  Uses an Instanced Static Mesh Component to batch-render all boids.
 */
UCLASS()
class BOIDS_PROJECT_API AVisualBoidSpecies : public AActor
{
	GENERATED_BODY()

public:
	
	AVisualBoidSpecies();
	
	void InitializeSpecies(UStaticMesh* BoidMesh, const int32 BoidsCount);
	void InitializeMaterial(UMaterialInstanceConstant* LoadedMaterialInstance);
	
	void ApplyMaterial();
	void UpdateMaterialColor(FLinearColor NewBoidColor);
	
	void UpdateBoidTransform(const int32 BoidIndex, const FVector& NewPosition, const FVector& NewRotation);
	void UpdateBoidTransforms();
	
	void AddBoids(const TArray<FTransform>& NewBoidTransforms);
	void RemoveBoids(const int32 CountToRemove);
	
	int32 Num() const;
	
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> VisualBoidMeshes;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BoidMaterial;	

	TArray<FTransform> VisualBoidTransforms;

	
private:
	
	void InitializeInstancedStaticMeshComponent();
	
	void AddBoidsInternal(const TArray<FTransform>& NewBoidTransforms);
	
};
