
#include "Utilities/ObjectPools/BoidPool.h"


FBoidPool::FBoidPool(const int32 InInitialSize, const int32 InRestockSize): RestockSize(InRestockSize)
{
	StoredBoids.Reserve(InInitialSize + InRestockSize);
	CurrentlyUsedBoids = 0;
	RestockBoids(InInitialSize);
}

void FBoidPool::AddBoids(const int32 BoidsCount)
{
	EnsureAvailableBoids(BoidsCount);
	check(CurrentlyUsedBoids + BoidsCount <= StoredBoids.Num())
	CurrentlyUsedBoids += BoidsCount;
}

void FBoidPool::RemoveLastUsed(const int32 BoidsCount)
{
	int32 MinimumAvailableBoids = FMath::Min(CurrentlyUsedBoids, BoidsCount);
	
	while (MinimumAvailableBoids > 0)
	{
		StoredBoids[--CurrentlyUsedBoids].Reset();
		MinimumAvailableBoids--;
	}
}

void FBoidPool::EnsureAvailableBoids(const int32 NeededBoids)
{
	if (StoredBoids.Num() - CurrentlyUsedBoids >= NeededBoids)
	{
		return;
	}
	
	const int32 NeededObjectsCount = RestockSize + ((NeededBoids - 1) / RestockSize) * RestockSize ;
	RestockBoids(NeededObjectsCount);
}

void FBoidPool::RestockBoids(const int32 CountToRestock)
{
	for (int i = 0; i < CountToRestock; i++)
	{
		StoredBoids.AddElement(FBoid());
	}
}
