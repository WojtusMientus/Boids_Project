#include "Visual/VisualBoidManager.h"

#include "Subsystems/BoidManagerSubsystem.h"
#include "Subsystems/BoidsManager.h"
#include "Visual/VisualBoid.h"
#include "Boids_Project/Globals.h"


AVisualBoidManager::AVisualBoidManager()
{
	PrimaryActorTick.bCanEverTick = true;

	LogicalBoidsManager = MakeShared<BoidsManager>();
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
		VisualBoid->AssignID(i);
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

void AVisualBoidManager::HandleBoidsUpdate()
{
	if (!BoidManagerSubsystem.IsValid())
	{
		return;
	}
	
	const TArray<TSharedPtr<Boid>>& Boids = BoidManagerSubsystem->GetBoids();

	// TODO -> IMPLEMENTATION 
}

void AVisualBoidManager::HandleBoidsNumberUpdate(EBoidType BoidType, int32 NewBoidNumber)
{
	// TODO -> IMPLEMENT THIS
}

void AVisualBoidManager::HandleBoidsColorUpdate(EBoidType BoidType, FColor NewBoidColor)
{
	// TODO -> IMPLEMENT THIS
}

