// Fill out your copyright notice in the Description page of Project Settings.

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
	AVisualBoidManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVisualBoid> VisualBoidClass;

private:

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

