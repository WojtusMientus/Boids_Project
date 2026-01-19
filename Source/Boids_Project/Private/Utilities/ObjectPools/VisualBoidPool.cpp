
#include "Utilities/ObjectPools/VisualBoidPool.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Visual/VisualBoid.h"


UVisualBoidPool::UVisualBoidPool()
{
}

void UVisualBoidPool::InitializeVisualBoidPool(const int32 InInitialSize, const int32 InRestockSize, 
	TSubclassOf<AVisualBoid> InVisualBoidClass, UWorld* InSimulationWorld)
{
	check(InSimulationWorld != nullptr)
	StoredVisualBoids.Reserve(InInitialSize + InRestockSize);
                                      	
	SimulationWorld = InSimulationWorld;
	RestockSize = InRestockSize;
	CurrentlyUsedBoids = 0;
	VisualBoidClass = InVisualBoidClass;
                                      	
	RestockVisualBoids(InInitialSize);
}

void UVisualBoidPool::AddVisualBoids(const int32 BoidsCount)
{
	EnsureAvailableVisualBoids(BoidsCount);
	CurrentlyUsedBoids += BoidsCount;
}

void UVisualBoidPool::RemoveLastUsed(const int32 BoidsCount)
{
	int32 MinimumAvailableBoids = FMath::Min(CurrentlyUsedBoids, BoidsCount);
	
	while (MinimumAvailableBoids > 0)
	{
		StoredVisualBoids[--CurrentlyUsedBoids]->Reset();
		MinimumAvailableBoids--;
	}
}

void UVisualBoidPool::EnsureAvailableVisualBoids(const int32 NeededVisualBoids)
{
	if (StoredVisualBoids.Num() - CurrentlyUsedBoids >= NeededVisualBoids)
	{
		return;
	}
	
	const int32 NeededObjectsCount = RestockSize + ((NeededVisualBoids - 1) / RestockSize) * RestockSize ;
	RestockVisualBoids(NeededObjectsCount);
}

void UVisualBoidPool::RestockVisualBoids(const int32 CountToRestock)
{
	ENSURE_ALWAYS_RETURN(SimulationWorld.IsValid())
	
	for (int i = 0; i < CountToRestock; i++)
	{
		AVisualBoid* SpawnedBoid = SimulationWorld->SpawnActor<AVisualBoid>(VisualBoidClass, FVector(), FRotator());
		check(SpawnedBoid != nullptr)		
		StoredVisualBoids.Add(SpawnedBoid);
		SpawnedBoid->Reset();
	}
}

