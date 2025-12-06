
#include "Core/Subsystems//BoidDataEditorSubsystem.h"
#include "Core/CollisionData.h"


void UBoidDataEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	BoidDataManager = MakeUnique<FEditorBoidDataManager>();
	CollisionDataGenerator = MakeUnique<FCollisionDataGenerator>();
}

void UBoidDataEditorSubsystem::InitializeNecessarySimulationData(FCollisionBoundsPlainData& BoundsData,
	TMap<FGameplayTag, FBoidsPlainData>& BoidsData)
{
	ENSURE_BOIDS_DATA_MANAGER()
	BoidDataManager->InitializeBoidSimulationData(BoundsData, BoidsData);
}

void UBoidDataEditorSubsystem::RegenerateCollisionDataAndSave(const FCollisionBoundsPlainData& CollisionBoundsData)
{
	ENSURE_BOIDS_DATA_MANAGER()
	ENSURE_COLLISION_DATA_MANAGER()
	FCollisionData CollisionData;
	CollisionData.CollisionBoundsData = CollisionBoundsData;
	CollisionDataGenerator->GenerateCollisionData(CollisionData);
	BoidDataManager->SaveBoundsData(CollisionBoundsData, CollisionData.CollisionForcesData);
	OnCollisionDataRegenerationEvent.Broadcast(CollisionData);
}

void UBoidDataEditorSubsystem::SaveBoidsData(const FBoidsPlainData& BoidsData)
{
	ENSURE_BOIDS_DATA_MANAGER()
	BoidDataManager->SaveBoidsData(BoidsData);
}

void UBoidDataEditorSubsystem::SaveAllBoidsData(const TMap<FGameplayTag, FBoidsPlainData>& AllBoidsData)
{
	ENSURE_BOIDS_DATA_MANAGER()
	BoidDataManager->SaveAllBoidsData(AllBoidsData);
}

void UBoidDataEditorSubsystem::HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent)
{
	OnSimulationBoundsChangedEvent.Broadcast(NewCenter, NewExtent);
}

void UBoidDataEditorSubsystem::HandleBoundsVisibilityChanged(bool bIsVisible)
{
	// OnBoundsVisibilityChangedEvent.Broadcast(bIsVisible);
}

void UBoidDataEditorSubsystem::HandleCollisionDataVisibilityChanged(bool bIsVisible)
{
	// OnCollisionDataVisibilityChangedEvent.Broadcast(bIsVisible);
}

