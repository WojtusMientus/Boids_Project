// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseClass/GameInstanceSubsystemBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/Boid.h"
#include "VoxelGrids/WorldCollisionVoxelGrid.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
#include "VoxelGrids/VoxelGridData/BoidCollisionCellData.h"
#include "VoxelGrids/BoidCollisionVoxelGrid.h"
#include "Core/BoidSpecies.h"
#include "Utilities/BoidNumberUpdateInfo.h"
#include "BoidManagerSubsystem.generated.h"

class URuntimeDataLoaderSubsystem;
struct FEnvironmentCollisionVoxelGridData;
class UBoidDataManagerSubsystem;


DECLARE_MULTICAST_DELEGATE(FOnBoidsInitializationFinishEvent);
DECLARE_MULTICAST_DELEGATE(FOnBoidsUpdateFinishEvent);
DECLARE_MULTICAST_DELEGATE(FOnBoidsNumberUpdateEvent);


/**
 * Main manager for Boid simulation.
 * Updates Boid positions and velocities each tick.
 * Handles neighbor queries and applies steering forces (Separation, Alignment, Cohesion).
 * Broadcasts events for visualization updates.
 */

// TODO: Future Optimization:
//		 - Adding a thread pool to parallelize Boid calculations.
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
	virtual void Deinitialize() override;
	//~ End UGameInstanceSubsystemBase Interface
	
	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	//~ End FTickableGameObject Interface
	
	
	FORCEINLINE FVector GetBoidPositionAt(int32 SpeciesID, int32 BoidID);
	FORCEINLINE FVector GetBoidVelocityAt(int32 SpeciesID, int32 BoidID);
	
	int32 GetDifferentBoidSpeciesCount() const;
	int32 GetBoidsSpeciesCount(int32 SpeciesIndex) const;

private:
		
	void CreateVoxelGrids();

	/** Initialization of simulation. */
	void InitializeSimulationData(UWorld* World, FWorldInitializationValues WorldInitializationValues);
	void InitializeSimulation(const TArray<FBoidsSpeciesPlainInfo>& BoidsInfo, 
							  const FEnvironmentCollisionVoxelGridData& EnvironmentCollisionVoxelGridData,
							  const FBoidCollisionVoxelGridData& BoidCollisionVoxelGridData);
	
	/** Initialization of Boids. */
	void InitializeDifferentBoidSpecies(const TArray<FBoidsSpeciesPlainInfo>& BoidsInfo);
	void InitializeBoidSpecies(const int32 SpeciesID);
	void InitializeBoidObject(const int32 SpeciesID, const int32 BoidID);
	
	/** Initialization of helper data structures. */
	void InitializeRemainingBoidSpeciesData();
	void InitializeBoidCollisionVoxelGrid(const FBoidCollisionVoxelGridData& BoidCollisionVoxelGridData);
	void InitializeNeighbourArray();
	
	
	void UpdateBoids(float DeltaTime);
	
	void CheckForAnyBoidNumberUpdate();
	void HandleBoidAddition(const int32 SpeciesID, const int32 CountToAdd);
	
	void RemakeBoidCollisionVoxelGrid();
	void GetNeighborBoidsDifferentSpeciesSorted_TwoArrays(const FBoid& Boid, const int SpeciesID);
	void GetNeighborBoidsDifferentSpeciesSorted_OneArray(const FBoid& Boid, const int SpeciesID);
	
	void GetNeighbourBoidsDifferentSpeciesSlow(const int32 SpeciesID, const int32 BoidID);
	void HandleSameSpeciesNeighborSearch(const int32 SpeciesID, const int32 BoidID);
	void HandleDifferentSpeciesNeighborSearch(const int32 SpeciesID, const int32 OtherSpeciesID, const int32 BoidID);
	
	void ComputeBoidBehaviourForces(FBoid& Boid, const uint8 SpeciesID, const uint16 BoidID);
	FVector ComputeSeparationMultipleSpecies(const int32 SpeciesID, const int32 BoidID);
	FVector ComputeAlignmentMultipleSpecies(const int32 SpeciesID);
	FVector ComputeCohesionMultipleSpecies(const int32 SpeciesID, const int32 BoidID);
	FVector ComputeForceBetweenDifferentSpecies(const int32 SpeciesID, const int32 BoidID);
	
	void ApplyEnvironmentCollisionForce(const int32 SpeciesID, const int32 BoidID);
	void ApplySpeedAdjustmentForcePerSpecies(const int32 SpeciesID, const int32 BoidID);
	void ApplyRecalculatedVelocity(float DeltaTime);
	
	
	bool IsWithinPerceptionRangeDifferentSpecies(const int32 CallerSpeciesID, const int32 CalledID,
	const int32 NeighborSpeciesID, const int32 NeighborID, const float DistanceSquared);
	
	
	
	static constexpr float SPEED_CORRECTION_FORCE = 0.85f;
		
	
	TArray<TUniquePtr<FBoidSpecies>> BoidSpecies;
	
	TArray<int32> SameSpeciesNeighbors;
	TArray<FBoidCollisionCellData> DifferentSpeciesNeighbors;
	
	TArray<uint16> SameNeighborsBoidDataSortedIf;
	TArray<FBoidCollisionCellData> DifferentNeighborsBoidDataSortedIf;
	
	TArray<TArray<uint16>> NeighborsBoidDataSortedNoIf;
	
	/** Weak reference to the BoidDataManager for getting converted gameplay tag to array index. */
	TWeakObjectPtr<UBoidDataManagerSubsystem> BoidDataManager;
	
	/** World collision bounds for boid to environment collision calculations. */
	TUniquePtr<FWorldCollisionVoxelGrid> WorldCollisionVoxelGrid;

	/** World collision bounds for boid to boid collision calculations. */
	TUniquePtr<FBoidCollisionVoxelGrid> BoidCollisionVoxelGrid;
	TUniquePtr<FBoidCollisionVoxelGrid> BoidCollisionVoxelGridTwo;
	
	
#if WITH_EDITOR
	void SubscribeToGlobalEditorDelegates();
	void UnsubscribeFromGlobalEditorDelegates();
	
	void HandleBoidNumberUpdateHandle(const FBoidNumberUpdateInfo& InBoidNumberUpdateInfo);
	
	void HandleBoidsForceParametersChange(const FGameplayTag Tag, float NewSeparationForce, float NewAlignmentForce,
	float NewCohesionForce, float NewOtherSpeciesMultiplier);
	
	void HandleBoidsSpatialAwarenessParametersChange(FGameplayTag Tag, float NewDesiredSpeed, 
		float NewPerceptionDistance);
	
	void HandleBoidCollisionMultiplierChange(FGameplayTag Tag, 	float NewEnvironmentCollisionMultiplier,
		float NewBoundsCollisionMultiplier);
	
	
	TQueue<FBoidNumberUpdateInfo> BoidNumberUpdateQueue;
	
	FDelegateHandle OnBoidNumberUpdateHandle;
	
	FDelegateHandle OnBoidForceParametersChangeHandle;
	FDelegateHandle OnBoidSpatialAwarenessParametersChangeHandle;
	FDelegateHandle OnBoidCollisionMultiplierUpdateHandle;
#endif
};
