// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseClass/GameInstanceSubsystemBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/Boid.h"
#include "Bounds/WorldCollisionBounds.h"
#include "Bounds/VoxelGrid/WorldCollisionVoxelGrid.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "BoidManagerSubsystem.generated.h"


class URuntimeDataLoaderSubsystem;
class FBoidCollisionVoxelGrid;
struct FEnvironmentCollisionVoxelGridData;
class UBoidDataManagerSubsystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoidsInitializationFinishEvent, const TArray<FBoidsPlainInfo> BoidsInfo);
DECLARE_MULTICAST_DELEGATE(FOnBoidsUpdateFinishEvent);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBoidsNumberUpdateEvent, FGameplayTag BoidType, int32 NewBoidNumber);

/**
 * Main manager for Boid simulation.
 * Updates Boid positions and velocities each tick.
 * Handles neighbor queries and applies steering forces (Separation, Alignment, Cohesion).
 * Broadcasts events for visualization updates.
 */

// TODO: Future Optimization:
//		 - Implementing Voxel Grid for spatial partitioning.
//		 - Loading saved data at startup.
//		 - Adding a thread pool to parallelize Boid calculations.
//		 - Implementing Collision Voxel Grid for collision detection without line tracing.
UCLASS()
class BOIDS_PROJECT_API UBoidManagerSubsystem : public UGameInstanceSubsystemBase, public FTickableGameObject
{
	GENERATED_BODY()
		
public:
	
	/** Delegate after Boid initialization complete. */
	FOnBoidsInitializationFinishEvent OnBoidsInitializationFinish;
	
	/** Delegate after Boid movement calculations complete. */
	FOnBoidsUpdateFinishEvent OnBoidsUpdateFinish;
	
	/** Delegate broadcast when the number of given Boid species changes. */
	FOnBoidsNumberUpdateEvent OnBoidsNumberUpdate;
	

	//~ Begin UGameInstanceSubsystemBase Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End UGameInstanceSubsystemBase Interface
	
	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	//~ End FTickableGameObject Interface
	
	
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
	
	
	/** Returns simulated Boid count. */
	FORCEINLINE int32 GetBoidsCount() { return Boids.Num(); }

	/** Updates internal Boid's IDs used in BoidCollisionVoxelGrid. */
	void UpdateBoidCellIndices(const int32 ID, const int32 VoxelGridIndex, const int32 VoxelGridCellIndex);


private:
	
	/** Desired movement speed that Boid is trying to achieve. */
	static constexpr float BOID_DESIRED_VELOCITY = 100.0f;

	/** Speed correction force multiplier.  */
	static constexpr float SPEED_CORRECTION_FORCE = 0.85f;
	
	/** Number of Boids spawned at simulation start. */
	static constexpr int32 BOIDS_COUNT = 10;
	
	/** Final multiplier applied to separation steering force. */
	static constexpr float SEPARATION_FORCE = 600.0f;
	
	/** Final multiplier applied to alignment steering force. */
	static constexpr float ALIGNMENT_FORCE = 200.0f;
	
	/** Final multiplier applied to cohesion steering force. */
	static constexpr float COHESION_FORCE = 600.0f;  
	
	/** Separation distance falloff. Stronger when Boids are closer. */
	static constexpr float SEPARATION_FALLOFF = 1.75f;

	/** Maximum perception radius. */
	static constexpr int32 PERCEPTION_DISTANCE = 150;

	/** Cached perception distance squared for distance calculations. */
	static constexpr int32 PERCEPTION_DISTANCE_SQUARED = PERCEPTION_DISTANCE * PERCEPTION_DISTANCE;

	/** Extent of the simulation bounds. */
	static constexpr int32 BOIDS_BOUNDS = 1000;
	
	
	
	void InitializeSimulation(const TArray<FBoidsPlainInfo> BoidsInfo, 
		const FEnvironmentCollisionVoxelGridData VoxelGridData);
	
	/** Populates the Boids array with the correct number of Boids. Called only during object creation. */
	void InitializeBoids(const TArray<FBoidsPlainInfo> BoidsInfo);
	void InitializeBoidsTest(const TArray<FBoidsPlainInfo> BoidsInfo);

	/** Initializes Boid's random startup position. */
	FVector CalculateBoidInitialPosition();

	/** Applies forces and updates Boid movement. */
	void UpdateBoids(float DeltaTime);
	void UpdateBoidsTest(float DeltaTime);
	
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
	void ApplyCollisionForceTest(FBoid* CurrentBoid);

	/**
	 * Applies corrective force towards its desired velocity.
	 * @param CurrentBoid Boid to apply the force to.
	 */
	void ApplySpeedAdjustmentForce(FBoid* CurrentBoid);
	void ApplySpeedAdjustmentForceTest(FBoid* CurrentBoid, int SpeciesIndex);
	
	/**
	 * Checks if 2 given Boids are within perception range.
	 * @param FirstIndex First Boid index.
	 * @param SecondIndex Second Boid index.
	 */
	bool IsWithinPerceptionRange(int32 FirstIndex, int32 SecondIndex);
	
	
	TArray<TArray<TUniquePtr<FBoid>>> DifferentSpeciesBoids; 
	TArray<TArray<FVector>> NewCalculatedVelocities;
	TArray<FBoidsPlainInfo> SpeciesInfo;
	
	TUniquePtr<FWorldCollisionVoxelGrid> WorldCollisionGrid;
	
	TWeakObjectPtr<UBoidDataManagerSubsystem> BoidDataManager;
	
	
	/** Array of all simulated Boids. */
	TArray<TUniquePtr<FBoid>> Boids;
	
	/** Cached neighbors of the currently calculated Boid. */
	TArray<FBoid*> CurrentNeighbours;
	
	/** Per-frame temporary array buffer for new velocities (applied after all forces are computed). */
	TArray<FVector> NewCalculatedVelocityPerBoid;
	
	/** World collision bounds for collision calculations. */
	TUniquePtr<FWorldCollisionBounds> WorldCollisionBounds;
	
	// TUniquePtr<FBoidCollisionVoxelGrid> BoidVoxelGrid;
};
