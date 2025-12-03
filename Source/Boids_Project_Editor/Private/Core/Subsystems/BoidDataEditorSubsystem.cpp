
#include "Core/Subsystems//BoidDataEditorSubsystem.h"
#include "DataAssets/BoundsData.h"


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
	
	TArray<bool> CollisionData = CollisionDataGenerator->GenerateCollisionData(CollisionBoundsData);
	BoidDataManager->SaveBoundsData(CollisionBoundsData, CollisionData);
	OnCollisionDataRegenerationEvent.Broadcast(CollisionData, CollisionBoundsData.BoundsPlainData);
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

