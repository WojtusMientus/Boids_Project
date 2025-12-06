
#include "Visual/VisualSimulationBoundsData.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Core/Subsystems/BoidDataEditorSubsystem.h"
#include "Visual/VisualizerVisibility.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"
#include "DataAssets/BoundsData.h"


AVisualSimulationBoundsData::AVisualSimulationBoundsData()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SimulationBoundsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounds Mesh"));
	SimulationBoundsMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SimulationBoundsMeshComponent->SetupAttachment(GetRootComponent());
	
	WallDataInstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>
(TEXT("WallDataInstancedStaticMeshComponent"));	
	WallDataInstancedStaticMeshComponent->SetupAttachment(RootComponent);
	WallDataInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	CollisionDataInstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>
(TEXT("CollisionDataInstancedStaticMeshComponent"));	
	CollisionDataInstancedStaticMeshComponent->SetupAttachment(RootComponent);
	CollisionDataInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
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

void AVisualSimulationBoundsData::TrySubscribeToSubsystemEvent()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_Transient | RF_ArchetypeObject) && GEditor && !bDidSubscribeToSubsystem)
	{
		if (UBoidDataEditorSubsystem* BoidDataEditorSubsystem = GEditor->GetEditorSubsystem<UBoidDataEditorSubsystem>())
		{
			BoundsChangedDelegateHandle = BoidDataEditorSubsystem->OnSimulationBoundsChangedEvent.AddUObject(this, 
				&AVisualSimulationBoundsData::HandleBoundsChanged);
			
			CollisionRegenerationDelegateHandle = BoidDataEditorSubsystem->OnCollisionDataRegenerationEvent.AddUObject
				(this, &AVisualSimulationBoundsData::HandleRegenerationCollisionData);
			
			AnyVisibilityChangedDelegateHandle = BoidDataEditorSubsystem->OnAnyVisibilityChanged.AddUObject(this, 
				&AVisualSimulationBoundsData::HandleAnyVisibilityChanged);
			
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
			BoidDataEditorSubsystem->OnSimulationBoundsChangedEvent.Remove(BoundsChangedDelegateHandle);
			BoidDataEditorSubsystem->OnCollisionDataRegenerationEvent.Remove(CollisionRegenerationDelegateHandle);
			BoidDataEditorSubsystem->OnAnyVisibilityChanged.Remove(AnyVisibilityChangedDelegateHandle);
			
			bDidSubscribeToSubsystem = false;
		}
	}
}

void AVisualSimulationBoundsData::HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent)
{
	ENSURE_BOUNDS_MESH_COMPONENT()
	SimulationBoundsMeshComponent->SetWorldLocation(NewCenter);
	
	const FVector NewBoundsSize = NewExtent * MeshScaleFactor;
	SimulationBoundsMeshComponent->SetRelativeScale3D(NewBoundsSize);
}

void AVisualSimulationBoundsData::HandleRegenerationCollisionData(const TArray<bool>& CollisionData,
	const FBoundsPlainData& BoundsData)
{
	ENSURE_WALL_DATA_INSTANCE_MESH_COMPONENT()
	ENSURE_COLLISION_DATA_INSTANCE_MESH_COMPONENT()
	
	WallDataInstancedStaticMeshComponent->ClearInstances();
	
	const FIntVector GridResolution = BoundsData.GridResolution;
	const FVector StartingPosition = FBoundsMath::GetStartingCellCenter(BoundsData.Center, BoundsData.Extent, 
	BoundsData.GridResolution);
	
	const FVector VoxelSize = FBoundsMath::GetVoxelCellSize(BoundsData.Center, BoundsData.Extent, 
		BoundsData.GridResolution);
	const FVector MeshScale = VoxelSize * MeshScaleFactor;

	
	for (int IndexX = 0; IndexX < GridResolution.X; IndexX++)
	{
		for (int IndexY = 0; IndexY < GridResolution.Y; IndexY++)
		{
			for (int IndexZ = 0; IndexZ < GridResolution.Z; IndexZ++)
			{
				if (!CollisionData[FBoundsMath::XYZToArrayIndex(IndexX, IndexY, IndexZ, GridResolution)])
				{
					continue;
				}
				
				const FVector Position = FBoundsMath::GetVoxelCenterAt(StartingPosition, VoxelSize, 
					IndexX, IndexY, IndexZ);
				
				const FTransform InstancedMeshTransform(FRotator::ZeroRotator, Position, MeshScale);
				WallDataInstancedStaticMeshComponent->AddInstance(InstancedMeshTransform, true);
			}
		}
	}
}

void AVisualSimulationBoundsData::HandleAnyVisibilityChanged(const FVisualizerVisibility& VisualizerVisibility)
{
	ENSURE_BOUNDS_MESH_COMPONENT()
	ENSURE_WALL_DATA_INSTANCE_MESH_COMPONENT()
	ENSURE_COLLISION_DATA_INSTANCE_MESH_COMPONENT()
	
	SimulationBoundsMeshComponent->SetVisibility(VisualizerVisibility.bIsBoundsVisible);
	WallDataInstancedStaticMeshComponent->SetVisibility(VisualizerVisibility.bIsWallDataVisible);
	CollisionDataInstancedStaticMeshComponent->SetVisibility(VisualizerVisibility.bIsCollisionDataVisible);
}
