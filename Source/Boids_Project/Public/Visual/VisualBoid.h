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
	void AssignID(int ID);
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BoidMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;

private:

	void AlignRotationToVelocity(const FVector& Velocity);

	int BoidID;
	
};
