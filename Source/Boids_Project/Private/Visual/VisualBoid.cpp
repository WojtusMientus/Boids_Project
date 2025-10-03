
#include "Visual/VisualBoid.h"
#include "Components/ArrowComponent.h"
#include "Core/BoidSelectionInfo.h"


AVisualBoid::AVisualBoid()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	BoidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boid Mesh"));
	BoidMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoidMeshComponent->SetupAttachment(GetRootComponent());

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(GetRootComponent());
}

void AVisualBoid::UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity)
{
	SetActorLocation(NewPosition);
	AlignRotationToVelocity(NewVelocity);
}

void AVisualBoid::SetBoidID(int32 ID)
{
	BoidID = ID;
}

FBoidSelectionInfo AVisualBoid::GetBoidSelection() const
{
	return FBoidSelectionInfo(BoidID, BoidType);
}

void AVisualBoid::AlignRotationToVelocity(const FVector& Velocity)
{
	const FRotator NewActorRotation = Velocity.Rotation();
	SetActorRotation(NewActorRotation);
}
