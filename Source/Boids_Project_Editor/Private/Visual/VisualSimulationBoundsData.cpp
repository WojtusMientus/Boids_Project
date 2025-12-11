
#include "Visual/VisualSimulationBoundsData.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Core/CollisionData.h"
#include "Core/Subsystems/BoidDataEditorSubsystem.h"
#include "Bounds/VoxelGrid/VoxelGridData/EnvironmentCollisionCellData.h"
#include "Visual/VisualizerVisibility.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"


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
	
	CollisionForcesDataInstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>
(TEXT("CollisionDataInstancedStaticMeshComponent"));	
	CollisionForcesDataInstancedStaticMeshComponent->SetupAttachment(RootComponent);
	CollisionForcesDataInstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	CollisionForcesDataInstancedStaticMeshComponent->CastShadow = false;
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
			
			AnyVisibilityChangedDelegateHandle = BoidDataEditorSubsystem->OnAnySimulationBoundsDataVisibilityChanged
			.AddUObject(this, &AVisualSimulationBoundsData::HandleAnyComponentVisibilityChanged);
			
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
			BoidDataEditorSubsystem->OnAnySimulationBoundsDataVisibilityChanged.Remove(AnyVisibilityChangedDelegateHandle);
			
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


void AVisualSimulationBoundsData::HandleRegenerationCollisionData(const FCollisionData& CollisionData)
{
	ENSURE_WALL_DATA_INSTANCE_MESH_COMPONENT()
	ENSURE_COLLISION_DATA_INSTANCE_MESH_COMPONENT()
	ENSURE_ALWAYS_MESSAGE_RETURN(CollisionData.EveryVoxelCenterData.Num() == CollisionData.CollisionForcesData.Num(),
		"Collision forces data is invalid!")
	
	WallDataInstancedStaticMeshComponent->ClearInstances();
	CollisionForcesDataInstancedStaticMeshComponent->ClearInstances();
	FBoundsPlainInfo BoundsData = CollisionData.CollisionBoundsData.BoundsPlainData;
	
	const FVector VoxelSize = FBoundsMath::GetVoxelCellSize(BoundsData.Center, BoundsData.Extent, 
		BoundsData.GridResolution);
	const FVector MeshScale = VoxelSize * MeshScaleFactor;
		
	for (const FVector& CellCenter: CollisionData.WallCollisionCentersData)
	{
		const FTransform InstancedMeshTransform(FRotator::ZeroRotator, CellCenter, MeshScale);
		WallDataInstancedStaticMeshComponent->AddInstance(InstancedMeshTransform, true);
	}
	
	for (int i = 0; i < CollisionData.CollisionForcesData.Num(); i++)
	{
		if (CollisionData.CollisionForcesData[i].EnvironmentCollisionForce.Length() == 0
			&& CollisionData.CollisionForcesData[i].BoundsCollisionForce.Length() == 0)
		{
			continue;
		}
		
		const FVector FinalForce = CollisionData.CollisionForcesData[i].EnvironmentCollisionForce + 
			CollisionData.CollisionForcesData[i].BoundsCollisionForce;
				
		const FVector CellCenter = CollisionData.EveryVoxelCenterData[i];
		
		const FTransform InstancedMeshTransform(FRotator::ZeroRotator, CellCenter, MeshScale);
		CollisionForcesDataInstancedStaticMeshComponent->AddInstance(InstancedMeshTransform, true);
	}
}

void AVisualSimulationBoundsData::HandleAnyComponentVisibilityChanged(const FVisualizerVisibility VisualizerVisibility)
{
	ENSURE_BOUNDS_MESH_COMPONENT()
	ENSURE_WALL_DATA_INSTANCE_MESH_COMPONENT()
	ENSURE_COLLISION_DATA_INSTANCE_MESH_COMPONENT()
	
	SimulationBoundsMeshComponent->SetVisibility(VisualizerVisibility.bIsBoundsVisible);
	WallDataInstancedStaticMeshComponent->SetVisibility(VisualizerVisibility.bIsWallDataVisible);
	CollisionForcesDataInstancedStaticMeshComponent->SetVisibility(VisualizerVisibility.bIsCollisionDataVisible);
}
