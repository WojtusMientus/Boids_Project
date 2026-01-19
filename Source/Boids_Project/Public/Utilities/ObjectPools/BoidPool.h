// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Core/Boid.h"


/**
 * Boid pool that manages storage and reuse of boids, tracking how many are currently active.
 */
class FBoidPool
{
	
public:
	
	FBoidPool(const int32 InInitialSize = 1000, const int32 InRestockSize = 100);
	
	void AddBoids(const int32 BoidsCount);
	void RemoveLastUsed(const int32 BoidsCount);
	
	FORCEINLINE int32 Num() const
	{
		return CurrentlyUsedBoids;
	}
	
	FORCEINLINE FBoid& operator[](const int32 Index)
	{
		check(Index < CurrentlyUsedBoids)
		return StoredBoids[Index];
	}
	
	
private:
	
	void EnsureAvailableBoids(const int32 NeededBoids);
	void RestockBoids(const int32 CountToRestock);
	
	TChunkedArray<FBoid> StoredBoids;
	
	int32 RestockSize = 100;
	int32 CurrentlyUsedBoids = 0;
};
