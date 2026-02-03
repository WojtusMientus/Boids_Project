
#include "Core/Subsystems/VisualBoidManagerSubsystem.h"
#include "Core/BoidDelegates.h"
#include "Core/Subsystems/BoidManagerSubsystem.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Settings/BoidSimulationSettings.h"
#include "Utilities/Macros/DebugMacros.h"
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
	NewVisualBoidsSpecies.Reserve(DifferentBoidSpeciesCount);
	
	UStaticMesh* VisualBoidMesh = GetVisualBoidMesh();
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentBoidSpeciesCount; SpeciesIndex++)
	{
		InitializeBoidSpecies(World, SpeciesIndex, VisualBoidMesh);
	}
}

void UVisualBoidManagerSubsystem::InitializeBoidSpecies(UWorld* World, const int SpeciesIndex, 
	UStaticMesh* VisualBoidMesh)
{
	TObjectPtr<AVisualBoidSpecies> NewVisualBoidSpecies = World->SpawnActor<AVisualBoidSpecies>(
		FVector(), FRotator());
	check(IsValid(NewVisualBoidSpecies))
	NewVisualBoidsSpecies.Add(NewVisualBoidSpecies);
	
	const int32 BoidCount = BoidManagerSubsystem->GetBoidsInSpeciesCount(SpeciesIndex);
	NewVisualBoidsSpecies[SpeciesIndex]->InitializeSpecies(VisualBoidMesh, BoidCount);
	UpdateBoidsTransformPerSpecies(SpeciesIndex);
}

void UVisualBoidManagerSubsystem::UpdateBoidsTransformPerSpecies(int32 SpeciesIndex)
{
	const int32 BoidCount = BoidManagerSubsystem->GetBoidsInSpeciesCount(SpeciesIndex);
	
	for (int BoidIndex = 0; BoidIndex < BoidCount; BoidIndex++)
	{
		const FVector NewPosition = BoidManagerSubsystem->GetBoidPositionAt(SpeciesIndex, BoidIndex);
		const FVector NewRotation = BoidManagerSubsystem->GetBoidVelocityAt(SpeciesIndex, BoidIndex);
		NewVisualBoidsSpecies[SpeciesIndex]->UpdateBoidTransform(BoidIndex, NewPosition, NewRotation);
	}
	
	NewVisualBoidsSpecies[SpeciesIndex]->UpdateBoidTransforms();
}

UStaticMesh* UVisualBoidManagerSubsystem::GetVisualBoidMesh()
{
	const UBoidSimulationSettings* SimulationSettings = GetDefault<UBoidSimulationSettings>();
	check(SimulationSettings != nullptr)
	return SimulationSettings->BoidMesh.LoadSynchronous();
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
	
	for (int SpeciesIndex = 0; SpeciesIndex < NewVisualBoidsSpecies.Num(); SpeciesIndex++)
	{
		NewVisualBoidsSpecies[SpeciesIndex]->InitializeMaterial(LoadedMaterialInstance);
		FLinearColor SpeciesColor = BoidDataManager->GetSpeciesColor(SpeciesIndex);
		NewVisualBoidsSpecies[SpeciesIndex]->UpdateMaterialColor(SpeciesColor);
	}
}

void UVisualBoidManagerSubsystem::HandleBoidsUpdate()
{
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	for (int SpeciesIndex = 0; SpeciesIndex < NewVisualBoidsSpecies.Num(); SpeciesIndex++)
	{
		UpdateBoidsTransformPerSpecies(SpeciesIndex);
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
	
	for (int SpeciesIndex = 0; SpeciesIndex < NewVisualBoidsSpecies.Num(); SpeciesIndex++)
	{
		const int32 LogicalBoidNumber = BoidManagerSubsystem->GetBoidsInSpeciesCount(SpeciesIndex);
		const int32 DifferenceInBoidNumber = LogicalBoidNumber - NewVisualBoidsSpecies[SpeciesIndex]->Num();
		
		if (DifferenceInBoidNumber < 0)
		{
			NewVisualBoidsSpecies[SpeciesIndex]->RemoveBoids(-DifferenceInBoidNumber);
		}
		else if (DifferenceInBoidNumber > 0)
		{
			HandleBoidAddition(SpeciesIndex, DifferenceInBoidNumber, LogicalBoidNumber);
		}
	}
	
	BroadcastOnBoidsNumberUpdateFinish();
}

void UVisualBoidManagerSubsystem::HandleBoidAddition(const int32 SpeciesID,	const int32 DifferenceInBoidNumber,
	const int32 NewBoidsCount)
{
	TArray<FTransform> NewlyAddedBoidsTransforms;
	NewlyAddedBoidsTransforms.Reserve(DifferenceInBoidNumber);
	
	for (int BoidIndex = NewBoidsCount - DifferenceInBoidNumber; BoidIndex < NewBoidsCount; BoidIndex++)
	{
		FTransform NewBoidTransform;
		NewBoidTransform.SetLocation(BoidManagerSubsystem->GetBoidPositionAt(SpeciesID, BoidIndex));
		NewBoidTransform.SetRotation(BoidManagerSubsystem->GetBoidVelocityAt(SpeciesID, BoidIndex).ToOrientationQuat());
		NewlyAddedBoidsTransforms.Add(NewBoidTransform);
	}
	
	NewVisualBoidsSpecies[SpeciesID]->AddBoids(NewlyAddedBoidsTransforms);
}

void UVisualBoidManagerSubsystem::BroadcastOnBoidsNumberUpdateFinish()
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	TArray<FBoidNumberUpdateInfo> BoidNumberUpdateInfos;
	BoidNumberUpdateInfos.Reserve(NewVisualBoidsSpecies.Num());
	
	for (int i = 0; i < NewVisualBoidsSpecies.Num(); i++)
	{
		const FGameplayTag BoidSpecies = BoidDataManager->RequestMappedKey(i);
		BoidNumberUpdateInfos.Add(FBoidNumberUpdateInfo(BoidSpecies, NewVisualBoidsSpecies[i]->Num()));
	}
	
	BoidsDelegates::OnBoidNumberUpdateFinish.Broadcast(BoidNumberUpdateInfos);
}

void UVisualBoidManagerSubsystem::HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(BoidType);
	ENSURE_ALWAYS_RETURN(NewVisualBoidsSpecies.IsValidIndex(MappedIndexFromGameplayTag))
	NewVisualBoidsSpecies[MappedIndexFromGameplayTag]->UpdateMaterialColor(NewBoidColor);
}

#endif
