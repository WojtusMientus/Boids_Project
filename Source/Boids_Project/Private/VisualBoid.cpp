// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualBoid.h"
#include "Components/ArrowComponent.h"


AVisualBoid::AVisualBoid()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BoidMeshCompoenent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidMesh"));
	BoidMeshCompoenent->SetupAttachment(GetRootComponent());

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(GetRootComponent());
}

void AVisualBoid::UpdateBoid(const FVector& NewPosition, const FVector& NewVelocity)
{
	SetActorLocation(NewPosition);
	AlignRotationToVelocity(NewVelocity);
}

void AVisualBoid::AlignRotationToVelocity(const FVector& Velocity)
{
	FRotator NewActorRotation = Velocity.Rotation();
	SetActorRotation(NewActorRotation);
}



