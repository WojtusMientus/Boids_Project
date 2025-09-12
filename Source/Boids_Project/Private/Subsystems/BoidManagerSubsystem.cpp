// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/BoidManagerSubsystem.h"

void UBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UBoidManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateBoids();
	OnBoidsUpdate.Broadcast();
}

TStatId UBoidManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTickableBoidManagerSubsystem, STATGROUP_Tickables);
}

void UBoidManagerSubsystem::UpdateBoids()
{
	// TODO - COPY EVERYTHING FROM MANAGER TO HERE
}
