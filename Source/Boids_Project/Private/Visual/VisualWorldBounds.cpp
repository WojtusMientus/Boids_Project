// Fill out your copyright notice in the Description page of Project Settings.


#include "Visual/VisualWorldBounds.h"
#include "Subsystems/BoidManagerSubsystem.h"

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
		UBoidManagerSubsystem* BoidManagerSubsystem = World->GetSubsystem<UBoidManagerSubsystem>();

		if (BoidManagerSubsystem)
		{
			BoidManagerSubsystem->OnBoundsUpdate.AddDynamic(this, &AVisualWorldBounds::HandleBoundsUpdate);
		}
	}
}

void AVisualWorldBounds::HandleBoundsUpdate(const FVector& Center, const FVector& Extent)
{
	// TODO - IMPLEMENTATION
}



