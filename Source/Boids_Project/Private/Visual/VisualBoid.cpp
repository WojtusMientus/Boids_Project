// Fill out your copyright notice in the Description page of Project Settings.


#include "Visual/VisualBoid.h"
#include "Components/ArrowComponent.h"


AVisualBoid::AVisualBoid()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	BoidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boid Mesh"));
	BoidMeshComponent->SetupAttachment(GetRootComponent());

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComponent->SetupAttachment(GetRootComponent());
}

void AVisualBoid::UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity)
{
	SetActorLocation(NewPosition);
	AlignRotationToVelocity(NewVelocity);
}

void AVisualBoid::AssignID(int ID)
{
	BoidID = ID;
}


void AVisualBoid::AlignRotationToVelocity(const FVector& Velocity)
{
	FRotator NewActorRotation = Velocity.Rotation();
	SetActorRotation(NewActorRotation);
}



