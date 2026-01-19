
#include "Visual/VisualSimulationBoundsData.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Core/CollisionData.h"
#include "Core/Subsystems/BoidDataEditorSubsystem.h"
#include "VoxelGrids/VoxelGridData/EnvironmentCollisionCellData.h"
#include "Visual/VisualizerVisibility.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"


AVisualSimulationBoundsData::AVisualSimulationBoundsData()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CreateSceneRootComponent();
	CreateSimulationBoundsMeshComponent();
	CreateWallDataInstancedStaticMeshComponent();
	CreateCollisionForcesDataInstancedStaticMeshComponent();
	CreateSimulationAreaVoxelMeshComponent();
}

void AVisualSimulationBoundsData::PostActorCreated()
{
	Super::PostActorCreated();
	TrySubscribeToSubsystemEvent();
}

void AVisualSimulationBoundsData::PostLoad()
{
	Super::PostLoad();
	TrySubscribeToSubsystemEvent();
}

void AVisualSimulationBoundsData::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TryToUnsubscribeFromSubsystemEvent();
}

void AVisualSimulationBoundsData::Destroyed()
{
	TryToUnsubscribeFromSubsystemEvent();	
	Super::Destroyed();
}

void AVisualSimulationBoundsData::CreateSceneRootComponent()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneRoot);
}

void AVisualSimulationBoundsData::CreateSimulationBoundsMeshComponent()
{
	SimulationBoundsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounds Mesh"));
	SimulationBoundsMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SimulationBoundsMeshComponent->SetupAttachment(GetRootComponent());
	check(SimulationBoundsMeshComponent)
}

void AVisualSimulationBoundsData::CreateWallDataInstancedStaticMeshComponent()
{
	WallDataInstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>
	(TEXT("WallDataInstancedStaticMeshComponent"));	
	WallDataInstancedStaticMeshComponent->SetupAttachment(RootComponent);
	WallDataInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	check(WallDataInstancedStaticMeshComponent)
}

void AVisualSimulationBoundsData::CreateCollisionForcesDataInstancedStaticMeshComponent()
{
	CollisionForcesDataInstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>
	(TEXT("CollisionDataInstancedStaticMeshComponent"));	
	CollisionForcesDataInstancedStaticMeshComponent->SetupAttachment(RootComponent);
	CollisionForcesDataInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	CollisionForcesDataInstancedStaticMeshComponent->CastShadow = false;
	check(CollisionForcesDataInstancedStaticMeshComponent)
}

void AVisualSimulationBoundsData::CreateSimulationAreaVoxelMeshComponent()
{
	SimulationAreaVoxelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Simulation Area Voxel"));
	SimulationAreaVoxelMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SimulationAreaVoxelMeshComponent->SetupAttachment(GetRootComponent());
	check(SimulationAreaVoxelMeshComponent)
}

void AVisualSimulationBoundsData::TrySubscribeToSubsystemEvent()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_Transient | RF_ArchetypeObject) && GEditor && !bDidSubscribeToSubsystem)
	{
		if (UBoidDataEditorSubsystem* BoidDataEditorSubsystem = GEditor->GetEditorSubsystem<UBoidDataEditorSubsystem>())
		{
			OnBoundsChangedHandle = BoidDataEditorSubsystem->OnSimulationBoundsChanged.AddUObject(this, 
				&AVisualSimulationBoundsData::HandleBoundsChanged);
			
			OnCollisionRegenerationHandle = BoidDataEditorSubsystem->OnCollisionDataRegeneration.AddUObject
				(this, &AVisualSimulationBoundsData::HandleRegenerationCollisionData);
			
			OnAnyVisibilityChangedHandle = BoidDataEditorSubsystem->OnAnySimulationBoundsDataVisibilityChanged
				.AddUObject(this, &AVisualSimulationBoundsData::HandleAnyComponentVisibilityChanged);
			
			OnSimulationAreaVoxelChangedHandle = BoidDataEditorSubsystem->OnSimulationAreaVoxelChanged.AddUObject
				(this, &AVisualSimulationBoundsData::HandleSimulationAreaVoxelChanged);
			
			bDidSubscribeToSubsystem = true;
		}
	}
}

void AVisualSimulationBoundsData::TryToUnsubscribeFromSubsystemEvent()
{
	if (!HasAnyFlags(RF_Transient) && GEditor && bDidSubscribeToSubsystem)
	{
		if (UBoidDataEditorSubsystem* BoidDataEditorSubsystem = GEditor->GetEditorSubsystem<UBoidDataEditorSubsystem>())
		{
			BoidDataEditorSubsystem->OnSimulationBoundsChanged.Remove(OnBoundsChangedHandle);
			BoidDataEditorSubsystem->OnCollisionDataRegeneration.Remove(OnCollisionRegenerationHandle);
			BoidDataEditorSubsystem->OnAnySimulationBoundsDataVisibilityChanged.Remove(OnAnyVisibilityChangedHandle);
			BoidDataEditorSubsystem->OnSimulationAreaVoxelChanged.Remove(OnSimulationAreaVoxelChangedHandle);
			
			bDidSubscribeToSubsystem = false;
		}
	}
}

void AVisualSimulationBoundsData::HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent)
{
	SimulationBoundsMeshComponent->SetWorldLocation(NewCenter);
	
	const FVector NewBoundsSize = NewExtent * MeshScaleFactor;
	SimulationBoundsMeshComponent->SetRelativeScale3D(NewBoundsSize);
}

void AVisualSimulationBoundsData::HandleRegenerationCollisionData(const FCollisionData& CollisionData)
{
	WallDataInstancedStaticMeshComponent->ClearInstances();
	CollisionForcesDataInstancedStaticMeshComponent->ClearInstances();
	FSimulationBoundsPlainInfo BoundsData = CollisionData.CollisionBoundsData.BoundsPlainData;
	
	const FVector VoxelCenterStart = FBoundsMath::GetStartingCellCenter(BoundsData.Center, BoundsData.Extent, 
		BoundsData.EnvironmentGridResolution);
	const FVector VoxelSize = FBoundsMath::GetVoxelCellSize(BoundsData.Center, BoundsData.Extent, 
		BoundsData.EnvironmentGridResolution);
	const FVector MeshScale = VoxelSize * MeshScaleFactor;
		
	for (int i = 0; i < CollisionData.CollisionForcesData.Num(); i++)
	{
		const FIntVector ArrayIndexVector = FBoundsMath::ArrayIndexToXYZ(i, BoundsData.EnvironmentGridResolution);
		const FVector CellCenter = FBoundsMath::GetVoxelCenterAt(VoxelCenterStart, VoxelSize, ArrayIndexVector);
		
		if (!CollisionData.CollisionForcesData[i].bIsSimulationArea)
		{
			const FTransform InstancedMeshTransform(FRotator::ZeroRotator, CellCenter, MeshScale);
			WallDataInstancedStaticMeshComponent->AddInstance(InstancedMeshTransform, true);
			continue;
		}
		
		if (CollisionData.CollisionForcesData[i].EnvironmentCollisionForce.Length() == 0
			&& CollisionData.CollisionForcesData[i].BoundsCollisionForce.Length() == 0)
		{
			continue;
		}
		
		const FTransform InstancedMeshTransform(FRotator::ZeroRotator, CellCenter, MeshScale);
		CollisionForcesDataInstancedStaticMeshComponent->AddInstance(InstancedMeshTransform, true);
	}
}

void AVisualSimulationBoundsData::HandleSimulationAreaVoxelChanged(
	const FCollisionBoundsPlainInfo& CollisionBoundsPlainInfo)
{
	const FVector VoxelCellSize = FBoundsMath::GetVoxelCellSize(CollisionBoundsPlainInfo.BoundsPlainData.Center,
		CollisionBoundsPlainInfo.BoundsPlainData.Extent, CollisionBoundsPlainInfo.BoundsPlainData.EnvironmentGridResolution);
	
	const FVector FinalPosition = GetSimulationVoxelStartPosition(CollisionBoundsPlainInfo.BoundsPlainData, 
		CollisionBoundsPlainInfo.SimulationSeedVoxelIndices);
	const FVector MeshScale = VoxelCellSize * MeshScaleFactor;
	
	SimulationAreaVoxelMeshComponent->SetWorldLocation(FinalPosition);
	SimulationAreaVoxelMeshComponent->SetWorldScale3D(MeshScale);
}

void AVisualSimulationBoundsData::HandleAnyComponentVisibilityChanged(const FVisualizerVisibility VisualizerVisibility)
{
	SimulationBoundsMeshComponent->SetVisibility(VisualizerVisibility.bIsBoundsVisible);
	WallDataInstancedStaticMeshComponent->SetVisibility(VisualizerVisibility.bIsWallDataVisible);
	CollisionForcesDataInstancedStaticMeshComponent->SetVisibility(VisualizerVisibility.bIsCollisionDataVisible);
	SimulationAreaVoxelMeshComponent->SetVisibility(VisualizerVisibility.bIsSimulationAreaVoxelVisible);
}

FVector AVisualSimulationBoundsData::GetSimulationVoxelStartPosition(
	const FSimulationBoundsPlainInfo& BoundsPlainData, const FIntVector& StartingIndices) const
{
	const FVector StartingCellCenter = FBoundsMath::GetStartingCellCenter(BoundsPlainData.Center,BoundsPlainData.Extent,
		BoundsPlainData.EnvironmentGridResolution);
	
	const FVector VoxelCellSize = FBoundsMath::GetVoxelCellSize(BoundsPlainData.Center,	BoundsPlainData.Extent,
		BoundsPlainData.EnvironmentGridResolution);
	
	const FIntVector AdjustedVoxelCell = StartingIndices - FIntVector(1);
	
	return FBoundsMath::GetVoxelCenterAt(StartingCellCenter, VoxelCellSize, AdjustedVoxelCell);
}

