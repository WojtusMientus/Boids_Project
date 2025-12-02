
#include "Visualizers/SimulationBoundsVisualizer.h"
#include "Core/Subsystems/BoidDataEditorSubsystem.h"


ASimulationBoundsVisualizer::ASimulationBoundsVisualizer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounds Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(GetRootComponent());
}

void ASimulationBoundsVisualizer::PostInitProperties()
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && GEditor)
	{
		if (UBoidDataEditorSubsystem* BoidDataEditorSubsystem = GEditor->GetEditorSubsystem<UBoidDataEditorSubsystem>())
		{
			BoidDataEditorSubsystem->OnSimulationBoundsChangedEvent.AddDynamic(this, 
				&ASimulationBoundsVisualizer::HandleBoundsChanged);
			
			BoidDataEditorSubsystem->OnBoundsVisibilityChangedEvent.AddDynamic(this, 
				&ASimulationBoundsVisualizer::HandleBoundsVisibilityChanged);
		}
	}
	
	Super::PostInitProperties();
}

void ASimulationBoundsVisualizer::HandleBoundsChanged(const FVector& NewCenter, const FVector& NewExtent)
{
	SetActorLocation(NewCenter);
	UpdateMeshBounds(NewExtent);
}

void ASimulationBoundsVisualizer::HandleBoundsVisibilityChanged(bool bNewVisibility)
{
	ENSURE_MESH_COMPONENT()
	MeshComponent->SetVisibility(bNewVisibility);
}

void ASimulationBoundsVisualizer::UpdateMeshBounds(const FVector& NewBoundsExtent)
{
	ENSURE_MESH_COMPONENT()
	
	const FVector NewBoundsSize = NewBoundsExtent * BoundsMeshScaleFactor;
	MeshComponent->SetRelativeScale3D(NewBoundsSize);
}


