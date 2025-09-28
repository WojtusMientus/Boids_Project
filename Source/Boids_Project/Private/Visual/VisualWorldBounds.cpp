
#include "Visual/VisualWorldBounds.h"
#include "Core/BoidManagerSubsystem.h"

AVisualWorldBounds::AVisualWorldBounds()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounds Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
}

void AVisualWorldBounds::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UBoidManagerSubsystem* BoidManagerSubsystem = World->GetSubsystem<UBoidManagerSubsystem>())
		{
			BoidManagerSubsystem->OnBoundsUpdate.AddDynamic(this, &AVisualWorldBounds::HandleBoundsUpdate);
		}
	}
}

void AVisualWorldBounds::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UWorld* World = GetWorld())
	{
		if (UBoidManagerSubsystem* BoidManagerSubsystem = World->GetSubsystem<UBoidManagerSubsystem>())
		{
			BoidManagerSubsystem->OnBoundsUpdate.RemoveDynamic(this, &AVisualWorldBounds::HandleBoundsUpdate);
		}
	}
}

void AVisualWorldBounds::HandleBoundsUpdate(const FVector& Center, const FVector& Extent)
{
	SetActorLocation(Center);
	UpdateMeshBounds(Extent);
}

void AVisualWorldBounds::UpdateMeshBounds(const FVector& NewBoundsExtent)
{
	const FVector NewBoundsSize = NewBoundsExtent * BoundsMeshScaleFactor;

	if (IsValid(MeshComponent))
	{
		MeshComponent->SetRelativeScale3D(NewBoundsSize);
	}
}
