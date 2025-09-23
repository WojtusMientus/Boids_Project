// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Boids_Project/Globals.h"
#include "Internal Logic/Boid.h"
#include "Internal Logic/Bounds/WorldCollisionBounds.h"
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
	
	UFUNCTION(BlueprintCallable)
	void PostAllActorsBeginPlay();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetPerceptionRadius() { return PERCEPTION_DISTANCE; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetBoidLocation(int32 Index) { return GetBoidPositionAt(Index); }
	
	
	static constexpr float SEPARATION_FORCE = .7;  
	static constexpr float ALIGNMENT_FORCE = .5;  
	static constexpr float COHESION_FORCE = .35;  

	static constexpr int32 PERCEPTION_DISTANCE = 125;
	static constexpr float BOID_MAX_SPEED = 250;
	
	static constexpr int32 BOIDS_COUNT = 0;
	static constexpr int32 BOIDS_BOUNDS = 1000;

	void InitializeBoids();
	void UpdateBoids();

	FVector GetBoidPositionAt(int32 Index);
	FVector GetBoidVelocityAt(int32 Index);

private:

	TArray<TSharedPtr<Boid>> GetNeighbourBoids(int32 BoidIndexToCheckNeighbours);
	void CheckBoidsSubarrayForValidBoids(int32 StartIndex, int32 EndIndex, int32 BoidIndexToCheckNeighbours, TArray<TSharedPtr<Boid>>& ValidBoids);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetNeighbourBoidsLocations(int32 BoidIndexToCheckNeighbours);
	
	void TestUpdateAllInOne();
	void ApplyCollisionForce();
	
	void UpdateSeparation();
	FVector SeparationResultPerBoid(int32 BoidIndexToCalculate);
	
	void UpdateAlignment();
	FVector AlignmentResultPerBoid(int32 BoidIndexToCalculate);
	
	void UpdateCohesion();
	FVector CohesionResultPerBoid(int32 BoidIndexToCalculate);

	bool IsInRange(int32 FirstIndex, int32 SecondIndex);
	

	float DeltaTimeSpeedModifier;
	
	TArray<TSharedPtr<Boid>> Boids;
	TArray<FVector> NewCalculatedVelocityPerBoid;
	
	TUniquePtr<FWorldCollisionBounds> WorldCollisionBounds;	
};
