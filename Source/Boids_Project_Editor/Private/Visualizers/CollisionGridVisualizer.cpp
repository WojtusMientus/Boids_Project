
#include "Visualizers/CollisionGridVisualizer.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Core/Subsystems/BoidDataEditorSubsystem.h"
#include "DataAssets/BoundsData.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"


ACollisionGridVisualizer::ACollisionGridVisualizer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	
	InstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>
	(TEXT("InstancedStaticMeshComponent"));	
	InstancedStaticMeshComponent->SetupAttachment(RootComponent);
	
	InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ACollisionGridVisualizer::PostInitProperties()
{
	Super::PostInitProperties();
	
	if (!HasAnyFlags(RF_ClassDefaultObject) && GEditor)
	{
		if (UBoidDataEditorSubsystem* BoidDataEditorSubsystem = GEditor->GetEditorSubsystem<UBoidDataEditorSubsystem>())
		{
			BoidDataEditorSubsystem->OnCollisionDataRegenerationEvent.AddDynamic(this, 
				&ACollisionGridVisualizer::HandleRegenerationCollisionData);
			
			BoidDataEditorSubsystem->OnCollisionDataVisibilityChangedEvent.	AddDynamic(this, 
				&ACollisionGridVisualizer::HandleCollisionDataVisibilityChanged);
		}
	}
}

void ACollisionGridVisualizer::HandleCollisionDataVisibilityChanged(bool bIsVisible)
{
	ENSURE_INSTANCE_MESH_COMPONENT();
	InstancedStaticMeshComponent->SetVisibility(bIsVisible);
}

void ACollisionGridVisualizer::HandleRegenerationCollisionData(const TArray<bool>& CollisionData,
                                                               const FBoundsPlainData& BoundsData)
{
	ENSURE_INSTANCE_MESH_COMPONENT();
	
	InstancedStaticMeshComponent->ClearInstances();
	
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
				InstancedStaticMeshComponent->AddInstance(InstancedMeshTransform, true);
			}
		}
	}
}
