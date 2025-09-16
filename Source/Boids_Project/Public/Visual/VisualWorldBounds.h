// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualWorldBounds.generated.h"

class UBoidManagerSubsystem;
class Bounds;


UCLASS()
class BOIDS_PROJECT_API AVisualWorldBounds : public AActor
{
	GENERATED_BODY()
	
public:	
	AVisualWorldBounds();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleBoundsUpdate(const FVector& Center, const FVector& Extent);

protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

private:

	void UpdateMeshBounds(const FVector& NewBoundsExtent);
	
};


