// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Boids_Project/Globals.h"
#include "Core/Boid.h"
#include "Bounds/WorldCollisionBounds.h"
#include "BoidManagerSubsystem.generated.h"


/** Delegates for visual updates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoidsUpdateFinishSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoundsUpdateSignature, const FVector&, NewCenter, const FVector&, Extent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoidsNumberUpdateSignature, EBoidType, BoidType, int32, NewBoidNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoidsColorUpdateSignature, EBoidType, BoidType, FColor, NewBoidColor);


/**
 * Main manager for Boid simulation.
 * Updates Boid positions and velocities each tick.
 * Handles neighbour queries and applies steering forces (Separation, Alignment, Cohesion).
 * Broadcasts events for visualization updates.
 */

// TODO: Future Optimization:
//		 - Implementing Voxel Grid for spatial partitioning.
//		 - Loading saved data at startup.
//		 - Adding a thread pool to parallelize Boid calculations.
//		 - Implementing Collision Voxel Grid for collision detection without line tracing.
UCLASS()
class BOIDS_PROJECT_API UBoidManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
		
public:

	/** Delegate after Boid movement calculations complete. */
	UPROPERTY()
	FOnBoidsUpdateFinishSignature OnBoidsUpdateFinish;
	
	/** Delegate broadcast when bounds center or extent changes. */
	UPROPERTY()
	FOnBoundsUpdateSignature OnBoundsUpdate;
	
	/** Delegate broadcast when the number of given Boid species changes. */
	UPROPERTY()
	FOnBoidsNumberUpdateSignature OnBoidsNumberUpdate;
	
	/** Delegate broadcast when the color of a given Boid species changes. */
	UPROPERTY()
	FOnBoidsColorUpdateSignature OnBoidsColorUpdate;

	//~ Begin UTickableWorldSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	//~ End UTickableWorldSubsystem Interface
	
	/**
	 * Returns Boid's world position at given index.
	 * @param Index Index of the Boid to query.
	 */
	FVector GetBoidPositionAt(int32 Index);

	/**
	 * Returns Boid's velocity at given index.
	 * @param Index Index of the Boid to query.
	 */
	FVector GetBoidVelocityAt(int32 Index);

	/** Called only from level blueprint after all actors in level are initialized. Broadcasts OnBoundsUpdate delegate for initial setup. */
	UFUNCTION(BlueprintCallable)
	void PostAllActorsBeginPlay();

	/** Returns perception radius used by Boids to find neighbors. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetPerceptionRadius() { return PERCEPTION_DISTANCE; }
	// TODO: Implement species-specific perception radius (linked to data assets).

	/**
	 * Returns world location of the Boid at given index.
	 * @param Index Index to sample on.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetBoidLocation(int32 Index) { return GetBoidPositionAt(Index); }

	/**
	 * Returns velocity of the Boid at given index.
	 * @param Index Index to sample on.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetBoidVelocity(int32 Index) { return GetBoidVelocityAt(Index); }
	
	/** Returns simulated Boid count. */
	FORCEINLINE int32 GetBoidsCount() { return BOIDS_COUNT; }
	
private:

	/** Maximum movement speed. */
	static constexpr float BOID_MAX_VELOCITY = 500.0f;

	/** Desired movement speed that Boid is trying to achieve. */
	static constexpr float BOID_DESIRED_VELOCITY = 150.0f;

	/** Speed correction force multiplier.  */
	static constexpr float SPEED_CORRECTION_FORCE = 0.01f;
	
	/** Number of Boids spawned at simulation start. */
	static constexpr int32 BOIDS_COUNT = 100;
	
	/** Final multiplier applied to separation steering force. */
	static constexpr float SEPARATION_FORCE = 125.0f;
	
	/** Final multiplier applied to alignment steering force. */
	static constexpr float ALIGNMENT_FORCE = 100.0f;
	
	/** Final multiplier applied to cohesion steering force. */
	static constexpr float COHESION_FORCE = 125.0f;  
	
	/** Separation distance falloff. Stronger when Boids are closer. */
	static constexpr float SEPARATION_FALLOFF = 1.5f;

	/** Maximum perception radius. */
	static constexpr int32 PERCEPTION_DISTANCE = 175;

	/** Cached perception distance squared for distance calculations. */
	static constexpr int32 PERCEPTION_DISTANCE_SQUARED = PERCEPTION_DISTANCE * PERCEPTION_DISTANCE;

	/** Extent of the simulation bounds. */
	static constexpr int32 BOIDS_BOUNDS = 1000;
	

	/** Populates the Boids array with the correct number of Boids. Called only during object creation. */
	void InitializeBoids();

	/** Initializes Boid's random startup position. */
	FVector CalculateBoidInitialPosition();

	/** Applies forces and updates Boid movement. */
	void UpdateBoids(float DeltaTime);

	/**
	 * Retrieves neighbouring Boids within perception radius.
	 * @param BoidIndexToCheckNeighbours Index of the Boid to calculate neighbours for.
	 * @param ValidBoids Output array of valid neighbouring Boids.
	 */
	void GetNeighbourBoids(int32 BoidIndexToCheckNeighbours, TArray<FBoid*>& ValidBoids);

	/**
	 * Retrieves neighbouring Boids within perception radius in subarray range.
	 * @param StartIndex Search start index.
	 * @param EndIndex End search index.
	 * @param BoidIndexToCheckNeighbours Index of the Boid to calculate neighbours for.
	 * @param ValidBoids Output array of valid neighbouring Boids.
	 */
	void CheckBoidsSubarrayForValidBoids(int32 StartIndex, int32 EndIndex, int32 BoidIndexToCheckNeighbours, TArray<FBoid*>& ValidBoids);

	/**
	 * Calculates Separation force for the given Boid.
	 * @param CurrentBoid The Boid being calculated.
	 */
	FVector ComputeSeparation(const FBoid* CurrentBoid);

	/** Calculates Alignment force from the current neighbours. */
	FVector ComputeAlignment();

	/**
	 * Calculates Cohesion force for the given Boid.
	 * @param CurrentBoid The Boid being calculated.
	 */
	FVector ComputeCohesion(const FBoid* CurrentBoid);

	/**
	 * Applies collision force to a given Boid.
	 * @param CurrentBoid Boid to apply the force to.
	 */
	void ApplyCollisionForce(FBoid* CurrentBoid);

	/**
	 * Applies corrective force towards its desired velocity.
	 * @param CurrentBoid Boid to apply the force to.
	 */
	void ApplySpeedAdjustmentForce(FBoid* CurrentBoid);
	
	/**
	 * Checks if 2 given Boids are within perception range.
	 * @param FirstIndex First Boid index.
	 * @param SecondIndex Second Boid index.
	 */
	bool IsWithinPerceptionRange(int32 FirstIndex, int32 SecondIndex);

	/**
	 * Returns an array of neighbouring Boid world positions (for visualization purposes). 
	 * @param BoidIndexToCheckNeighbours Index of the Boid to query neighbours from.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetNeighbourBoidsLocations(int32 BoidIndexToCheckNeighbours);
	
	/** Array of all simulated Boids. */
	TArray<TUniquePtr<FBoid>> Boids;
	
	/** Cached neighbours of the currently calculated Boid. */
	TArray<FBoid*> CurrentNeighbours;
	
	/** Per-frame temporary array buffer for new velocities (applied after all forces are computed). */
	TArray<FVector> NewCalculatedVelocityPerBoid;
	
	/** World collision bounds for collision calculations. */
	TUniquePtr<FWorldCollisionBounds> WorldCollisionBounds;	
};
