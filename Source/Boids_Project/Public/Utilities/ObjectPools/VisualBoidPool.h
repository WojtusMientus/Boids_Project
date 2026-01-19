// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VisualBoidPool.generated.h"

class AVisualBoid;


/**
 * Visual boid pool that manages storage and reuse of boid actors, tracking how many are currently active.
 */
UCLASS()
class BOIDS_PROJECT_API UVisualBoidPool : public UObject
{
	GENERATED_BODY()
	
public:

	UVisualBoidPool();
	
	void InitializeVisualBoidPool(const int32 InInitialSize, const int32 InRestockSize, 
		TSubclassOf<AVisualBoid> InVisualBoidClass, UWorld* InSimulationWorld);
	
	void AddVisualBoids(const int32 BoidsCount);
	void RemoveLastUsed(const int32 BoidsCount);
	
	FORCEINLINE int32 Num() const
	{
		return CurrentlyUsedBoids;
	}
	
	FORCEINLINE AVisualBoid* Get(int32 Index)
	{
		check(Index < CurrentlyUsedBoids && StoredVisualBoids[Index])
		return StoredVisualBoids[Index];
	}
	
	
private:
	
	void EnsureAvailableVisualBoids(const int32 NeededVisualBoids);
	void RestockVisualBoids(const int32 CountToRestock);
	
	
	/** Weak pointer for spawning actors in correct world. */
	TWeakObjectPtr<UWorld> SimulationWorld; 
	
	UPROPERTY()
	TArray<TObjectPtr<AVisualBoid>> StoredVisualBoids;
	
	UPROPERTY()
	TSubclassOf<AVisualBoid> VisualBoidClass;
	
	
	int32 RestockSize = 100;
	int32 CurrentlyUsedBoids = 0;
};
