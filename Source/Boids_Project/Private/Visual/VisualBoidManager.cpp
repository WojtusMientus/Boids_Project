#include "Visual/VisualBoidManager.h"

#include "Subsystems/BoidManagerSubsystem.h"
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

		FVector StartingLocation = BoidManagerSubsystem->GetBoidPositionAt(i);
		FVector StartingVelocity = BoidManagerSubsystem->GetBoidVelocityAt(i);
		VisualBoid->UpdateBoid(StartingLocation, StartingVelocity);
		
		VisualBoid->AssignID(i);
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


void AVisualBoidManager::HandleBoidsNumberUpdate(EBoidType BoidType, int32 NewBoidNumber)
{
	// TODO -> IMPLEMENT THIS
}

void AVisualBoidManager::HandleBoidsColorUpdate(EBoidType BoidType, FColor NewBoidColor)
{
	// TODO -> IMPLEMENT THIS
}

