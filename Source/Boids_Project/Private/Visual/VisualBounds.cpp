// Fill out your copyright notice in the Description page of Project Settings.


#include "Visual/VisualBounds.h"
#include "Subsystems/BoidManagerSubsystem.h"

AVisualBounds::AVisualBounds()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bounds Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	
}

void AVisualBounds::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		UBoidManagerSubsystem* BoidManagerSubsystem = World->GetSubsystem<UBoidManagerSubsystem>();

		if (BoidManagerSubsystem)
		{
			BoidManagerSubsystem->OnBoundsUpdate.AddDynamic(this, &AVisualBounds::HandleBoundsUpdate);
		}
	}
}

void AVisualBounds::HandleBoundsUpdate(const FVector& Center, const FVector& Extent)
{
	// TODO - IMPLEMENTATION
}



