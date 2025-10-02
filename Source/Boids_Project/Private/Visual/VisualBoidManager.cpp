
#include "Visual/VisualBoidManager.h"
#include "Core/BoidManagerSubsystem.h"
#include "Visual/VisualBoid.h"
#include "Boids_Project/Globals.h"


AVisualBoidManager::AVisualBoidManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AVisualBoidManager::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		BoidManagerSubsystem = World->GetSubsystem<UBoidManagerSubsystem>();
	
		if (BoidManagerSubsystem.IsValid())
		{			
			BoidManagerSubsystem->OnBoidsUpdate.AddDynamic(this, &AVisualBoidManager::HandleBoidsUpdate);
			BoidManagerSubsystem->OnBoidsNumberUpdate.AddDynamic(this, &AVisualBoidManager::HandleBoidsNumberUpdate);
			BoidManagerSubsystem->OnBoidsColorUpdate.AddDynamic(this, &AVisualBoidManager::HandleBoidsColorUpdate);
		}
	}

	InitializeBoids();
}

void AVisualBoidManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (BoidManagerSubsystem.IsValid())
	{			
		BoidManagerSubsystem->OnBoidsUpdate.RemoveDynamic(this, &AVisualBoidManager::HandleBoidsUpdate);
		BoidManagerSubsystem->OnBoidsNumberUpdate.RemoveDynamic(this, &AVisualBoidManager::HandleBoidsNumberUpdate);
		BoidManagerSubsystem->OnBoidsColorUpdate.RemoveDynamic(this, &AVisualBoidManager::HandleBoidsColorUpdate);
	}
}

void AVisualBoidManager::InitializeBoids()
{
	TObjectPtr<UWorld> World = GetWorld();

	if (!IsValid(World) || !BoidManagerSubsystem.IsValid())
	{
		return;
	}
	
	for (int i = 0; i < UBoidManagerSubsystem::BOIDS_COUNT; i++)
	{
		TObjectPtr<AVisualBoid> VisualBoid = World->SpawnActor<AVisualBoid>(VisualBoidClass, FVector(), FRotator());

		if (!IsValid(VisualBoid))
		{
			continue;
		}
		
		FVector StartingLocation = BoidManagerSubsystem->GetBoidPositionAt(i);
		FVector StartingVelocity = BoidManagerSubsystem->GetBoidVelocityAt(i);
		VisualBoid->UpdateBoid(StartingLocation, StartingVelocity);
		VisualBoid->SetBoidID(i);
		
		VisualBoids.Add(VisualBoid);
	}
}

void AVisualBoidManager::HandleBoidsUpdate()
{
	if (!BoidManagerSubsystem.IsValid())
	{
		return;
	}
	
	for (int i = 0; i < UBoidManagerSubsystem::BOIDS_COUNT; i++)
	{
		FVector NewLocation = BoidManagerSubsystem->GetBoidPositionAt(i);
		FVector NewVelocity = BoidManagerSubsystem->GetBoidVelocityAt(i);
		VisualBoids[i]->UpdateBoid(NewLocation, NewVelocity);
	}
}

void AVisualBoidManager::HandleBoidsNumberUpdate(EBoidType BoidType, int32 NewBoidCount)
{
	// TODO: Implement spawning and despawning visual boids
}

void AVisualBoidManager::HandleBoidsColorUpdate(EBoidType BoidType, FColor NewBoidColor)
{
	// TODO: Implement visual color update
}
