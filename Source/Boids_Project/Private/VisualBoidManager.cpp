#include "VisualBoidManager.h"
#include "BoidsManager.h"
#include "VisualBoid.h"

AVisualBoidManager::AVisualBoidManager()
{
	PrimaryActorTick.bCanEverTick = true;

	LogicalBoidsManager = MakeShared<BoidsManager>();
}

void AVisualBoidManager::BeginPlay()
{
	Super::BeginPlay();
	LogicalBoidsManager->InitializeBoids();
	InitializeBoids();
}

void AVisualBoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LogicalBoidsManager->UpdateBoids();
	UpdateBoids();
}

void AVisualBoidManager::InitializeBoids()
{
	TObjectPtr<UWorld> World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}
	
	for (int i = 0; i < BoidsManager::BOIDS_COUNT; i++)
	{
		TObjectPtr<AVisualBoid> VisualBoid = World->SpawnActor<AVisualBoid>(VisualBoidClass, FVector(), FRotator());

		FVector StartingLocation = LogicalBoidsManager->GetBoidPositionAt(i);
		FVector StartingVelocity = LogicalBoidsManager->GetBoidVelocityAt(i);

		VisualBoid->UpdateBoid(StartingLocation, StartingVelocity);
		VisualBoids.Add(VisualBoid);
	}
}

void AVisualBoidManager::UpdateBoids()
{
	for (int i = 0; i < BoidsManager::BOIDS_COUNT; i++)
	{
		FVector NewLocation = LogicalBoidsManager->GetBoidPositionAt(i);
		FVector NewVelocity = LogicalBoidsManager->GetBoidVelocityAt(i);
		VisualBoids[i]->UpdateBoid(NewLocation, NewVelocity);
	}
}

