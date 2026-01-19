// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VisualBoidSpecies.generated.h"

class UVisualBoidPool;
class AVisualBoid;
class UMaterialInstanceConstant;


/**
 *  Runtime container for visual boids of a single species and their material.
 */
UCLASS()
class BOIDS_PROJECT_API UVisualBoidSpecies : public UObject
{
	GENERATED_BODY()

public:
	
	UVisualBoidSpecies() {}
	
	void InitializeSpecies(const int32 BoidCount, TSubclassOf<AVisualBoid> VisualBoidClass, UWorld* InSimulationWorld);
	void InitializeMaterial(UMaterialInstanceConstant* LoadedMaterialInstance);
	
	void ApplyMaterial(const int32 StartIndex, const int32 EndIndex);
	void UpdateMaterialColor(FLinearColor NewColor);
	
	/** Visual boid pool wrappers. */
	AVisualBoid* Get(const int32 BoidIndex);
	
	void AddBoids(const int32 CountToAdd);
	void RemoveBoids(const int32 CountToRemove);
	
	int32 Num() const;
	
	
	UPROPERTY()
	TObjectPtr<UVisualBoidPool> VisualBoidPool;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BoidMaterial;	
};
