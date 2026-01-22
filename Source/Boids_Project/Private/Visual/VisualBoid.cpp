
#include "Visual/VisualBoid.h"


AVisualBoid::AVisualBoid()
{
	PrimaryActorTick.bCanEverTick = false;

	CreateRootSceneComponent();
	CreateBoidMeshComponent();
}

void AVisualBoid::Initialize(const FVector& InitPosition, const FVector& InitVelocity, UMaterialInterface* InitMaterial)
{
	SetActorHiddenInGame(false);
	UpdateBoid(InitPosition, InitVelocity);
	SetMaterial(InitMaterial);
}

void AVisualBoid::Reset()
{
	SetActorHiddenInGame(true);
	SetActorLocation(FVector::Zero());
	SetActorRotation(FRotator());
}

void AVisualBoid::UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity)
{
	SetActorLocation(NewPosition);
	AlignRotationToVelocity(NewVelocity);
}

void AVisualBoid::SetMaterial(UMaterialInterface* NewMaterial)
{
	BoidMeshComponent->SetMaterial(0, NewMaterial);
}

void AVisualBoid::CreateRootSceneComponent()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);
	check(SceneRoot)
}

void AVisualBoid::CreateBoidMeshComponent()
{
	BoidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boid Mesh"));
	BoidMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoidMeshComponent->SetupAttachment(GetRootComponent());
	check(BoidMeshComponent)
}


void AVisualBoid::AlignRotationToVelocity(const FVector& Velocity)
{
	const FRotator NewActorRotation = Velocity.Rotation();
	SetActorRotation(NewActorRotation);
}
