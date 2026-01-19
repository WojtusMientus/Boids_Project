
#include "Core/Subsystems//BoidDataEditorSubsystem.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Core/CollisionData.h"


void UBoidDataEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	BoidDataManager = MakeUnique<FEditorBoidDataManager>();
	CollisionDataGenerator = MakeUnique<FCollisionDataGenerator>();
}

void UBoidDataEditorSubsystem::InitializeNecessarySimulationData(FCollisionBoundsPlainInfo& BoundsData,
	TMap<FGameplayTag, FBoidsSpeciesPlainInfo>& BoidsData)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	BoidDataManager->InitializeBoidSimulationData(BoundsData, BoidsData);
}

void UBoidDataEditorSubsystem::RegenerateCollisionDataAndSave(const FCollisionBoundsPlainInfo& CollisionBoundsData)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	ENSURE_ALWAYS_RETURN(CollisionDataGenerator.IsValid())
	
	FCollisionData CollisionData;
	CollisionData.CollisionBoundsData = CollisionBoundsData;
	CollisionDataGenerator->GenerateCollisionData_NEW(CollisionData);
	BoidDataManager->SaveBoundsData(CollisionBoundsData, CollisionData.CollisionForcesData);
	OnCollisionDataRegeneration.Broadcast(CollisionData);
}

void UBoidDataEditorSubsystem::SaveBoidsData(const FBoidsSpeciesPlainInfo& BoidsData)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	BoidDataManager->SaveBoidsData(BoidsData);
}

void UBoidDataEditorSubsystem::SaveAllBoidsData(const TMap<FGameplayTag, FBoidsSpeciesPlainInfo>& AllBoidsData)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	BoidDataManager->SaveAllBoidsData(AllBoidsData);
}

void UBoidDataEditorSubsystem::HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent)
{
	OnSimulationBoundsChanged.Broadcast(NewCenter, NewExtent);
}

void UBoidDataEditorSubsystem::HandleSimulationBoundsDataVisibilityChanged(const FVisualizerVisibility VisualizerVisibility)
{
	OnAnySimulationBoundsDataVisibilityChanged.Broadcast(VisualizerVisibility);
}

void UBoidDataEditorSubsystem::HandleSimulationAreaVoxelChanged(
	const FCollisionBoundsPlainInfo& CollisionBoundsPlainInfo)
{
	OnSimulationAreaVoxelChanged.Broadcast(CollisionBoundsPlainInfo);
}
