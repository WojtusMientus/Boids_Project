// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Boids_Project/Globals.h"
#include "Internal Logic/Boid.h"
#include "BoidManagerSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoidsUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoundsUpdateSignature, const FVector&, NewCenter, const FVector&, Extent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoidsNumberUpdateSignature, EBoidType, BoidType, int32, NewBoidNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoidsColorUpdateSignature, EBoidType, BoidType, FColor, NewBoidColor);

UCLASS()
class BOIDS_PROJECT_API UBoidManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	// For VisualBoidManager to get array and update visual boids
	UPROPERTY()
	FOnBoidsUpdateSignature OnBoidsUpdate;

	// For VisualBounds to update at runtime
	UPROPERTY()
	FOnBoundsUpdateSignature OnBoundsUpdate;

	// For VisualBoidManager to either delete or create new boids
	UPROPERTY()
	FOnBoidsNumberUpdateSignature OnBoidsNumberUpdate;

	// For VisualBoidManager to change material property or maybe material Instance (will have to go deeper into it to understand)
	UPROPERTY()
	FOnBoidsColorUpdateSignature OnBoidsColorUpdate;


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	FORCEINLINE const TArray<TSharedPtr<Boid>>& GetBoids() { return Boids; }

private:

	void UpdateBoids();
	
	TArray<TSharedPtr<Boid>> Boids;
	
	TArray<FVector> NewCalculatedVelocityPerBoid;	
};
