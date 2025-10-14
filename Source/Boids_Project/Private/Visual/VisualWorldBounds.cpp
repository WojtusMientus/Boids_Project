
#include "Visual/VisualWorldBounds.h"
#include "Core/BoidManagerSubsystem.h"

AVisualWorldBounds::AVisualWorldBounds()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounds Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(GetRootComponent());
}

void AVisualWorldBounds::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		BoidManagerSubsystem = World->GetSubsystem<UBoidManagerSubsystem>();
		
		if (BoidManagerSubsystem.IsValid())
		{
			BoidManagerSubsystem->OnBoundsUpdate.AddDynamic(this, &AVisualWorldBounds::HandleBoundsUpdate);
		}
	}
}

void AVisualWorldBounds::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (BoidManagerSubsystem.IsValid())
	{
		BoidManagerSubsystem->OnBoundsUpdate.RemoveDynamic(this, &AVisualWorldBounds::HandleBoundsUpdate);
	}
}

void AVisualWorldBounds::HandleBoundsUpdate(const FVector& NewCenter, const FVector& NewExtent)
{
	SetActorLocation(NewCenter);
	UpdateMeshBounds(NewExtent);
}

void AVisualWorldBounds::UpdateMeshBounds(const FVector& NewBoundsExtent)
{
	if (!ensure(IsValid(MeshComponent)))
	{
		return;
	}
	
	const FVector NewBoundsSize = NewBoundsExtent * BoundsMeshScaleFactor;
	MeshComponent->SetRelativeScale3D(NewBoundsSize);
}
