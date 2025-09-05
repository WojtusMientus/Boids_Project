// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualBoid.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class BOIDS_PROJECT_API AVisualBoid : public AActor
{
	GENERATED_BODY()
	
public:	
	AVisualBoid();

	void UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity);

	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BoidMeshCompoenent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;

private:

	void AlignRotationToVelocity(const FVector& Velocity);
	
};
