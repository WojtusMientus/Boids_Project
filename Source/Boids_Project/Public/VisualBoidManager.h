// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidsManager.h"
#include "VisualBoidManager.generated.h"


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
		
public:	
	virtual void Tick(float DeltaTime) override;

private:

	void InitializeBoids();
	void UpdateBoids();

	
	UPROPERTY(VisibleInstanceOnly)
	TArray<TObjectPtr<AVisualBoid>> VisualBoids;

	TSharedPtr<BoidsManager> LogicalBoidsManager;
	
};

