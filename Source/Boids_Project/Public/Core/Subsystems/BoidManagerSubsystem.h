// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseClass/GameInstanceSubsystemBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/Boid.h"
#include "VoxelGrids/WorldCollisionVoxelGrid.h"
#include "DataAssets/SimulationPlainInfoData/BoidsPlainInfoData.h"
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
	
	
	FORCEINLINE FVector GetBoidPositionAt(int32 SpeciesID, int32 BoidID)
	{
		return BoidSpecies[SpeciesID]->BoidPool[BoidID].Position;
	}
	FORCEINLINE FVector GetBoidVelocityAt(int32 SpeciesID, int32 BoidID)
	{
		return BoidSpecies[SpeciesID]->BoidPool[BoidID].Velocity;
	}
	
	FORCEINLINE int32 GetDifferentBoidSpeciesCount() const
	{
		return BoidSpecies.Num();
	}
	
	FORCEINLINE int32 GetBoidsInSpeciesCount(int32 SpeciesIndex) const
	{
		return BoidSpecies[SpeciesIndex]->Num();
	}


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
	
	void RemakeBoidCollisionVoxelGrid();
	void GetNeighborBoidsDifferentSpeciesVoxelGrid(const FVector& BoidsPositon, const int SpeciesID);
	
	void ApplyAllBoidBehaviourForces(FBoid& Boid, const uint8 SpeciesID, const uint16 BoidID);
	void ComputeBoidBehaviorForces(const int32 SpeciesID, const int32 BoidID, FVector& SeparationVector, 
		FVector& AlignmentVector, FVector& CohesionVector);
	
	void AddSeparationForcePerNeighbor(const FVector& DesiredDirection, const float DistanceSquared, 
		const float PerceptionDistance,	FVector& SeparationVector);
	void AddAlignmentForcePerNeighbor(const FVector& OtherBoidVelocity, FVector& AlignmentVector);
	void AddCohesionForcePerNeighbor(const FVector& OtherBoidPosition, FVector& CohesionVector);
	
	void ComputeFinalSeparationForce(const float SeparationMultiplier, FVector& SeparationVector, 
		const int32 CumulativeNumberOfNeighbors);
	void ComputeFinalAlignmentForce(const float AlignmentMultiplier, FVector& AlignmentVector, 
		const int32 CumulativeNumberOfNeighbors);
	void ComputeFinalCohesionForce(const FVector& CurrentBoidPosition, const float CohesionMultiplier, 
		FVector& CohesionVector, const int32 CumulativeNumberOfNeighbors);
	
	FVector ComputeForceBetweenDifferentSpecies(const int32 SpeciesID, const int32 BoidID);
	void AddForceBetweenDifferentSpecies(const float OtherSpeciesID, const FVector& CurrentBoidPosition,
		FVector& OtherSpeciesForceVector);
	void ComputeFinalBetweenDifferentSpeciesForce(const int32 SpeciesID, 
		const int32 CumulativeNumberOfOtherSpeciesNeighbors, FVector& DifferentSpeciesForceVector);
	
	
	void ApplyEnvironmentCollisionForce(FBoid& CurrentBoid, const float EnvironmentCollisionMultiplier, 
		const float BoundsCollisionMultiplier);
	void ApplySpeedAdjustmentForcePerSpecies(FBoid& CurrentBoid, const float DesiredSpeed);
	void ApplyRecalculatedVelocity(float DeltaTime);
	
	
	FORCEINLINE bool IsWithinPerceptionRange(const float DistanceSquaredToOtherBoid, 
		const float PerceptionDistanceSquared) const
	{
		return DistanceSquaredToOtherBoid <= PerceptionDistanceSquared;
	}
		
	
	static constexpr float SPEED_CORRECTION_FORCE = 0.85f;
	
	
	TArray<TUniquePtr<FBoidSpecies>> BoidSpecies;
	
	TArray<TArray<uint16>> NeighborsBoidData;
	
	/** Weak reference to the BoidDataManager for getting converted gameplay tag to array index. */
	TWeakObjectPtr<UBoidDataManagerSubsystem> BoidDataManager;
	
	/** World collision bounds for boid to environment collision calculations. */
	TUniquePtr<FWorldCollisionVoxelGrid> WorldCollisionVoxelGrid;

	/** World collision bounds for boid to boid collision calculations. */
	TUniquePtr<FBoidCollisionVoxelGrid> BoidCollisionVoxelGrid;
	
	
#if WITH_EDITOR
	void CheckForAnyBoidNumberUpdate();
	void HandleBoidAddition(const int32 SpeciesID, const int32 CountToAdd);
	
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
