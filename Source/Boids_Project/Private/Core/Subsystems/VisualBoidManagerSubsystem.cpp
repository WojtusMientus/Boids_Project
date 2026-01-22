
#include "Core/Subsystems/VisualBoidManagerSubsystem.h"
#include "Core/BoidDelegates.h"
#include "Core/Subsystems/BoidManagerSubsystem.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Settings/BoidSimulationSettings.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Visual/VisualBoid.h"
#include "Visual/VisualBoidSpecies.h"


void UVisualBoidManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World))
	ENSURE_ALWAYS_RETURN(World->IsGameWorld())
	
	Super::Initialize(Collection);
	Collection.InitializeDependency(UBoidManagerSubsystem::StaticClass());
	Collection.InitializeDependency(UBoidDataManagerSubsystem::StaticClass());

	UGameInstance* GameInstance = World->GetGameInstance();
	ENSURE_ALWAYS_RETURN(IsValid(GameInstance))
	
	BoidManagerSubsystem = GameInstance->GetSubsystem<UBoidManagerSubsystem>();
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	BoidManagerSubsystem->OnBoidsInitializationFinish.AddUObject(this, 
		&UVisualBoidManagerSubsystem::HandleVisualSimulationInitialization);
	
	BoidDataManager = GameInstance->GetSubsystem<UBoidDataManagerSubsystem>();
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	BindNecessarySimulationDelegates();
}

void UVisualBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
#if WITH_EDITOR
	UnsubscribeFromGlobalEditorDelegates();
#endif
}

void UVisualBoidManagerSubsystem::BindNecessarySimulationDelegates()
{
	BoidManagerSubsystem->OnBoidsUpdateFinish.AddUObject(this, 
		&UVisualBoidManagerSubsystem::HandleBoidsUpdate);
	
#if WITH_EDITOR
	BoidManagerSubsystem->OnBoidsNumberUpdate.AddUObject(this, 
		&UVisualBoidManagerSubsystem::HandleBoidsNumberUpdate);
	
	SubscribeToGlobalEditorDelegates();
#endif
}

void UVisualBoidManagerSubsystem::HandleVisualSimulationInitialization()
{
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	InitializeAllBoidsSpecies();
	RequestMaterialInstanceAsset();
}

void UVisualBoidManagerSubsystem::InitializeAllBoidsSpecies()
{
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World) && BoidManagerSubsystem.IsValid())
	
	const int32 DifferentBoidSpeciesCount = BoidManagerSubsystem->GetDifferentBoidSpeciesCount(); 
	VisualBoidsSpecies.Reserve(DifferentBoidSpeciesCount);
	
	TSubclassOf<AVisualBoid> VisualBoidClass = GetVisualBoidClass();
	
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentBoidSpeciesCount; SpeciesIndex++)
	{
		InitializeBoidSpecies(SpeciesIndex, VisualBoidClass);
	}
}

void UVisualBoidManagerSubsystem::InitializeBoidSpecies(const int SpeciesIndex, const TSubclassOf<AVisualBoid> VisualBoidClass)
{
	const int32 BoidCount = BoidManagerSubsystem->GetBoidsSpeciesCount(SpeciesIndex);
	VisualBoidsSpecies.Add(NewObject<UVisualBoidSpecies>());
	check(VisualBoidsSpecies[SpeciesIndex] != nullptr);
	
	VisualBoidsSpecies[SpeciesIndex]->InitializeSpecies(BoidCount, VisualBoidClass, GetWorld());
	
	for (int BoidIndex = 0; BoidIndex < VisualBoidsSpecies[SpeciesIndex]->Num(); BoidIndex++)
	{
		InitializeVisualBoid(SpeciesIndex, BoidIndex);
	}
}

void UVisualBoidManagerSubsystem::InitializeVisualBoid(int32 SpeciesIndex, int32 BoidIndex)
{
	AVisualBoid* VisualBoid = VisualBoidsSpecies[SpeciesIndex]->Get(BoidIndex);
	FVector StartingLocation = BoidManagerSubsystem->GetBoidPositionAt(SpeciesIndex, BoidIndex);
	FVector StartingVelocity = BoidManagerSubsystem->GetBoidVelocityAt(SpeciesIndex, BoidIndex);
	
	VisualBoid->Initialize(StartingLocation, StartingVelocity);
}

TSubclassOf<AVisualBoid> UVisualBoidManagerSubsystem::GetVisualBoidClass()
{
	const UBoidSimulationSettings* SimulationSettings = GetDefault<UBoidSimulationSettings>();
	check(SimulationSettings != nullptr)
	return SimulationSettings->VisualBoidClass;
}

void UVisualBoidManagerSubsystem::RequestMaterialInstanceAsset()
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	BoidDataManager->OnBoidMaterialInstanceLoaded.AddUObject(this, 
	&UVisualBoidManagerSubsystem::HandleLoadedMaterialAssets);
	BoidDataManager->RequestMaterialInstanceAsset();
}

void UVisualBoidManagerSubsystem::HandleLoadedMaterialAssets(UMaterialInstanceConstant* LoadedMaterialInstance)
{
	ENSURE_ALWAYS_RETURN(IsValid(LoadedMaterialInstance))
	InitializeBoidMaterials(LoadedMaterialInstance);
}

void UVisualBoidManagerSubsystem::InitializeBoidMaterials(UMaterialInstanceConstant* LoadedMaterialInstance)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	for (int SpeciesIndex = 0; SpeciesIndex < VisualBoidsSpecies.Num(); SpeciesIndex++)
	{
		VisualBoidsSpecies[SpeciesIndex]->InitializeMaterial(LoadedMaterialInstance);
		FLinearColor SpeciesColor = BoidDataManager->GetSpeciesColor(SpeciesIndex);
		VisualBoidsSpecies[SpeciesIndex]->UpdateMaterialColor(SpeciesColor);
	}
}

void UVisualBoidManagerSubsystem::HandleBoidsUpdate()
{
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(BoidManager_Update_Visual)
		
		ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
		for (int SpeciesIndex = 0; SpeciesIndex < VisualBoidsSpecies.Num(); SpeciesIndex++)
		{
			for (int BoidIndex = 0 ; BoidIndex < VisualBoidsSpecies[SpeciesIndex]->Num(); BoidIndex++)
			{
				AVisualBoid* CurrentBoid = VisualBoidsSpecies[SpeciesIndex]->Get(BoidIndex);			
				FVector NewLocation = BoidManagerSubsystem->GetBoidPositionAt(SpeciesIndex, BoidIndex);
				FVector NewVelocity = BoidManagerSubsystem->GetBoidVelocityAt(SpeciesIndex, BoidIndex);
					
				CurrentBoid->UpdateBoid(NewLocation, NewVelocity);
			}
		}
	}
}

#if WITH_EDITOR
void UVisualBoidManagerSubsystem::SubscribeToGlobalEditorDelegates()
{
	BoidsColorUpdateDelegateHandle = BoidsDelegates::OnBoidColorUpdate.AddUObject(this,  
		&UVisualBoidManagerSubsystem::HandleBoidsColorUpdate);
}

void UVisualBoidManagerSubsystem::UnsubscribeFromGlobalEditorDelegates()
{
	BoidsDelegates::OnBoidColorUpdate.Remove(BoidsColorUpdateDelegateHandle);
}

void UVisualBoidManagerSubsystem::HandleBoidsNumberUpdate()
{
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	for (int SpeciesIndex = 0; SpeciesIndex < VisualBoidsSpecies.Num(); SpeciesIndex++)
	{
		const int32 LogicalBoidNumber = BoidManagerSubsystem->GetBoidsSpeciesCount(SpeciesIndex);
		const int32 DifferenceInBoidNumber = LogicalBoidNumber - VisualBoidsSpecies[SpeciesIndex]->Num();
		
		if (DifferenceInBoidNumber < 0)
		{
			VisualBoidsSpecies[SpeciesIndex]->RemoveBoids(-DifferenceInBoidNumber);
		}
		else if (DifferenceInBoidNumber > 0)
		{
			HandleBoidAddition(SpeciesIndex, DifferenceInBoidNumber);
		}
	}
	
	BroadcastOnBoidsNumberUpdateFinish();
}

void UVisualBoidManagerSubsystem::HandleBoidAddition(const int32 SpeciesID, const int32 CountToAdd)
{
	const int32 NumberOfBoidsBeforeAddition = VisualBoidsSpecies[SpeciesID]->Num();
	VisualBoidsSpecies[SpeciesID]->AddBoids(CountToAdd);
	const int32 NumberAfterAddition = NumberOfBoidsBeforeAddition + CountToAdd;
	
	for (int BoidIndex = NumberOfBoidsBeforeAddition; BoidIndex < NumberAfterAddition; BoidIndex++)
	{
		InitializeVisualBoid(SpeciesID, BoidIndex);
	}
	
	VisualBoidsSpecies[SpeciesID]->ApplyMaterial(NumberOfBoidsBeforeAddition, NumberAfterAddition);
}

void UVisualBoidManagerSubsystem::BroadcastOnBoidsNumberUpdateFinish()
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	TArray<FBoidNumberUpdateInfo> BoidNumberUpdateInfos;
	BoidNumberUpdateInfos.Reserve(VisualBoidsSpecies.Num());
	
	for (int i = 0; i < VisualBoidsSpecies.Num(); i++)
	{
		const FGameplayTag BoidSpecies = BoidDataManager->RequestMappedKey(i);
		BoidNumberUpdateInfos.Add(FBoidNumberUpdateInfo(BoidSpecies, VisualBoidsSpecies[i]->Num()));
	}
	
	BoidsDelegates::OnBoidNumberUpdateFinish.Broadcast(BoidNumberUpdateInfos);
}

void UVisualBoidManagerSubsystem::HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(BoidType);
	ENSURE_ALWAYS_RETURN(VisualBoidsSpecies.IsValidIndex(MappedIndexFromGameplayTag))
	VisualBoidsSpecies[MappedIndexFromGameplayTag]->UpdateMaterialColor(NewBoidColor);
}

#endif
