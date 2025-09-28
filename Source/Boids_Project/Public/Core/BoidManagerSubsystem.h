
// Main manager for Boid simulation
// Updates Boid positions and velocities each tick 
// Broadcasts events for visual updates
//
// Future Optimization:
//		1. Implementing Voxel Grid for spatial partitioning 
//		2. Adding a thread pool to parallelize Boid calculations
//		3. Implementing Collision Voxel Grid for collision detection without line tracing

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Boids_Project/Globals.h"
#include "Core/Boid.h"
#include "Bounds/WorldCollisionBounds.h"
#include "BoidManagerSubsystem.generated.h"


// ----- Delegates for visual updates -----
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoidsUpdateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoundsUpdateSignature, const FVector&, NewCenter, const FVector&, Extent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoidsNumberUpdateSignature, EBoidType, BoidType, int32, NewBoidNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoidsColorUpdateSignature, EBoidType, BoidType, FColor, NewBoidColor);

UCLASS()
class BOIDS_PROJECT_API UBoidManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	// ----- Events for visual updates -----
	UPROPERTY()
	FOnBoidsUpdateSignature OnBoidsUpdate;
	
	UPROPERTY()
	FOnBoundsUpdateSignature OnBoundsUpdate;
	
	UPROPERTY()
	FOnBoidsNumberUpdateSignature OnBoidsNumberUpdate;
	
	UPROPERTY()
	FOnBoidsColorUpdateSignature OnBoidsColorUpdate;

	// ----- UTickableWorldSubsystem Overrides -----
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	// ----- Public API -----
	FVector GetBoidPositionAt(int32 Index);
	FVector GetBoidVelocityAt(int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void PostAllActorsBeginPlay();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetPerceptionRadius() { return PERCEPTION_DISTANCE; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetBoidLocation(int32 Index) { return GetBoidPositionAt(Index); }

	
	static constexpr float BOID_MAX_SPEED = 250;
	static constexpr int32 BOIDS_COUNT = 100;
	
private:

	static constexpr float SEPARATION_FORCE = .85f;  
	static constexpr float ALIGNMENT_FORCE = .5f;  
	static constexpr float COHESION_FORCE = .4f;  

	static constexpr float SEPARATION_DESIRED_DIRECTION_MULTIPLIER = 1.5f;
	
	static constexpr int32 PERCEPTION_DISTANCE = 125;
	static constexpr int32 PERCEPTION_DISTANCE_SQUARED = PERCEPTION_DISTANCE * PERCEPTION_DISTANCE;

	static constexpr int32 BOIDS_BOUNDS = 1000;
	

	// ----- Private Simulation Methods -----
	void InitializeBoids();
	void UpdateBoids(float DeltaTime);
	
	void GetNeighbourBoids(int32 BoidIndexToCheckNeighbours, TArray<Boid*>& ValidBoids);
	void CheckBoidsSubarrayForValidBoids(int32 StartIndex, int32 EndIndex, int32 BoidIndexToCheckNeighbours, TArray<Boid*>& ValidBoids);

	// ----- Calculating Forces Per Boid -----
	FVector ComputeSeparation(const Boid* CurrentBoid);
	FVector ComputeAlignment();
	FVector ComputeCohesion(const Boid* CurrentBoid);
	void ApplyCollisionForce(int32 BoidIndex);
	
	bool IsWithinPerceptionRange(int32 FirstIndex, int32 SecondIndex);

	// ----- Private Helper for ISelectableBoid -----
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetNeighbourBoidsLocations(int32 BoidIndexToCheckNeighbours);
	
	
	TArray<TUniquePtr<Boid>> Boids;
	TArray<Boid*> CurrentNeighbours;
	
	TArray<FVector> NewCalculatedVelocityPerBoid;
	
	TUniquePtr<FWorldCollisionBounds> WorldCollisionBounds;	
};
