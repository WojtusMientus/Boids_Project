// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subsystems/BoidsManager.h"
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
		
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetBoidPerceptionRadius() { return BoidsManager::PERCEPTION_DISTANCE; }
	
private:

	void InitializeBoids();
	void UpdateBoids();

	void HandleBoidsUpdate();
	void HandleBoidsNumberUpdate(EBoidType BoidType, int32 NewBoidNumber);
	void HandleBoidsColorUpdate(EBoidType BoidType, FColor NewBoidColor);
	
	TWeakObjectPtr<UBoidManagerSubsystem> BoidManagerSubsystem;
	
	UPROPERTY(VisibleInstanceOnly)
	TArray<TObjectPtr<AVisualBoid>> VisualBoids;

	TSharedPtr<BoidsManager> LogicalBoidsManager;
	
};

