
#include "Visual/VisualBoidManager.h"
#include "Core/BoidManagerSubsystem.h"
#include "Visual/VisualBoid.h"


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
			BoidManagerSubsystem->OnBoidsUpdateFinish.AddDynamic(this, &AVisualBoidManager::HandleBoidsUpdate);
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
		BoidManagerSubsystem->OnBoidsUpdateFinish.RemoveDynamic(this, &AVisualBoidManager::HandleBoidsUpdate);
		BoidManagerSubsystem->OnBoidsNumberUpdate.RemoveDynamic(this, &AVisualBoidManager::HandleBoidsNumberUpdate);
		BoidManagerSubsystem->OnBoidsColorUpdate.RemoveDynamic(this, &AVisualBoidManager::HandleBoidsColorUpdate);
	}
}

void AVisualBoidManager::InitializeBoids()
{
	UWorld* World = GetWorld();

	if (!IsValid(World) || !BoidManagerSubsystem.IsValid())
	{
		return;
	}

	const int32 NumberOfBoids = BoidManagerSubsystem->GetBoidsCount(); 
	
	for (int i = 0; i < NumberOfBoids; i++)
	{
		AVisualBoid* VisualBoid = World->SpawnActor<AVisualBoid>(VisualBoidClass, FVector(), FRotator());

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

	const int32 NumberOfBoids = BoidManagerSubsystem->GetBoidsCount(); 
	
	for (int i = 0; i < NumberOfBoids; i++)
	{
		FVector NewLocation = BoidManagerSubsystem->GetBoidPositionAt(i);
		FVector NewVelocity = BoidManagerSubsystem->GetBoidVelocityAt(i);
		VisualBoids[i]->UpdateBoid(NewLocation, NewVelocity);
	}
}

void AVisualBoidManager::HandleBoidsNumberUpdate(FGameplayTag BoidType, int32 NewBoidCount)
{
	// TODO: Implement spawning and despawning visual boids
}

void AVisualBoidManager::HandleBoidsColorUpdate(FGameplayTag BoidType, FColor NewBoidColor)
{
	// TODO: Implement visual color update
}
