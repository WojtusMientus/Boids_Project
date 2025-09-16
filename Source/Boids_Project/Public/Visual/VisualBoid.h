// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SelectableBoid.h"
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
	AVisualBoid();

	void UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity);
	void AssignID(int32 ID);

	virtual FBoidSelection OnBoidSelection() override;
	
protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BoidMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;

private:

	void AlignRotationToVelocity(const FVector& Velocity);

	int32 BoidID;

	EBoidType BoidType;
};
