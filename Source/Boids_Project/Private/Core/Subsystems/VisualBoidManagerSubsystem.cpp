
#include "Core/Subsystems/VisualBoidManagerSubsystem.h"
#include "Core/BoidDelegates.h"
#include "Core/Subsystems/BoidManagerSubsystem.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Settings/BoidSimulationSettings.h"
#include "Utilities/BoidConstants.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Visual/VisualBoid.h"


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
	
	
	GetDataFromSimulationSettings();
	BindNecessaryDelegates();
	
#if WITH_EDITOR
	BoidsColorChangeDelegateHandle = BoidsDelegates::OnBoidColorUpdate.AddUObject(this,  
		&UVisualBoidManagerSubsystem::HandleBoidsColorUpdate);
#endif
	
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UVisualBoidManagerSubsystem::PostWorldInitialized);
}

void UVisualBoidManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
#if WITH_EDITOR
	BoidsDelegates::OnBoidColorUpdate.Remove(BoidsColorChangeDelegateHandle);
#endif
}

void UVisualBoidManagerSubsystem::HandleVisualSimulationInitialization()
{
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	if (bIsVisualSimulationReadyToInitialize && !bIsVisualSimulationInitialized)
	{
		InitializeBoidsTest();
		RequestMaterialInstanceAsset();
	}
}

void UVisualBoidManagerSubsystem::PostWorldInitialized(UWorld* World,
	FWorldInitializationValues WorldInitializationValues)
{
	bIsVisualSimulationReadyToInitialize = true;
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	if (BoidManagerSubsystem->IsSimulationReady() && !bIsVisualSimulationInitialized)
	{
		InitializeBoidsTest();
		RequestMaterialInstanceAsset();
	}
}

void UVisualBoidManagerSubsystem::GetDataFromSimulationSettings()
{
	const UBoidSimulationSettings* SimulationSettings = GetDefault<UBoidSimulationSettings>();
	ENSURE_ALWAYS_RETURN(IsValid(SimulationSettings))
	
	VisualBoidClass = SimulationSettings->VisualBoidClass;
}

void UVisualBoidManagerSubsystem::BindNecessaryDelegates()
{
	BoidManagerSubsystem->OnBoidsUpdateFinish.AddUObject(this, 
		&UVisualBoidManagerSubsystem::HandleBoidsUpdateTest);
	BoidManagerSubsystem->OnBoidsNumberUpdate.AddUObject(this, 
		&UVisualBoidManagerSubsystem::HandleBoidsNumberUpdate);
}

void UVisualBoidManagerSubsystem::InitializeBoids()
{
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World) && BoidManagerSubsystem.IsValid())

	const int32 NumberOfBoids = BoidManagerSubsystem->GetBoidsCount(); 
	
	for (int i = 0; i < NumberOfBoids; i++)
	{
		AVisualBoid* VisualBoid = World->SpawnActor<AVisualBoid>(VisualBoidClass, FVector(), FRotator());		
		InitializeBoid(VisualBoid, i);
	}
}

void UVisualBoidManagerSubsystem::InitializeBoid(AVisualBoid* BoidToInitialize, int32 BoidIndex)
{
	ENSURE_ALWAYS_RETURN(IsValid(BoidToInitialize))
	
	FVector StartingLocation = BoidManagerSubsystem->GetBoidPositionAt(BoidIndex);
	FVector StartingVelocity = BoidManagerSubsystem->GetBoidVelocityAt(BoidIndex);
	BoidToInitialize->UpdateBoid(StartingLocation, StartingVelocity);
		
	VisualBoids.Add(BoidToInitialize);
}

void UVisualBoidManagerSubsystem::InitializeBoidsTest()
{
	bIsVisualSimulationInitialized = true;
	UWorld* World = GetWorld();
	ENSURE_ALWAYS_RETURN(IsValid(World) && BoidManagerSubsystem.IsValid())
	
	const int32 DifferentBoidSpeciesCount = BoidManagerSubsystem->GetDifferentBoidSpeciesCount(); 
	DifferentSpeciesBoids.Reserve(DifferentBoidSpeciesCount);
	
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentBoidSpeciesCount; SpeciesIndex++)
	{
		const int32 BoidCount = BoidManagerSubsystem->GetBoidsSpeciesCount(SpeciesIndex);
		
		DifferentSpeciesBoids.Add(TArray<TObjectPtr<AVisualBoid>>());
		DifferentSpeciesBoids.Reserve(BoidCount);
		
		for (int i = 0; i < BoidCount; i++)
		{
			AVisualBoid* VisualBoid = World->SpawnActor<AVisualBoid>(VisualBoidClass, FVector(), FRotator());		
			InitializeBoidTest(VisualBoid, SpeciesIndex, i);
		}
	}
}

void UVisualBoidManagerSubsystem::InitializeBoidTest(AVisualBoid* BoidToInitialize, int32 SpeciesIndex, int32 BoidIndex)
{
	ENSURE_ALWAYS_RETURN(IsValid(BoidToInitialize))
	
	FVector StartingLocation = BoidManagerSubsystem->GetBoidPositionAtTest(SpeciesIndex, BoidIndex);
	FVector StartingVelocity = BoidManagerSubsystem->GetBoidVelocityAtTest(SpeciesIndex, BoidIndex);
	BoidToInitialize->UpdateBoid(StartingLocation, StartingVelocity);
		
	DifferentSpeciesBoids[SpeciesIndex].Add(BoidToInitialize);
}

void UVisualBoidManagerSubsystem::RequestMaterialInstanceAsset()
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	BoidDataManager->OnBoidMaterialInstanceLoaded.AddUObject(this, 
	&UVisualBoidManagerSubsystem::HandleLoadedMaterialAsset);
	BoidDataManager->RequestMaterialInstanceAsset();
}

void UVisualBoidManagerSubsystem::HandleLoadedMaterialAsset(UMaterialInstanceConstant* LoadedMaterialInstance)
{
	ENSURE_ALWAYS_RETURN(IsValid(LoadedMaterialInstance))
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	const int32 DifferentBoidSpeciesCount = BoidManagerSubsystem->GetDifferentBoidSpeciesCount(); 
	BoidMaterials.Reserve(DifferentBoidSpeciesCount);
	
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentBoidSpeciesCount; SpeciesIndex++)
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(
			LoadedMaterialInstance, this);
		ENSURE_ALWAYS_RETURN(IsValid(DynamicMaterialInstance))
		
		FLinearColor SpeciesColor = BoidDataManager->GetSpeciesColor(SpeciesIndex);
		DynamicMaterialInstance->SetVectorParameterValue(FName(FBoidConstants::BoidMaterialColorParameter), 
			SpeciesColor);
		
		BoidMaterials.Add(DynamicMaterialInstance);
	}
	
	
	for (int SpeciesIndex = 0; SpeciesIndex <  DifferentSpeciesBoids.Num(); SpeciesIndex++)
	{
		ENSURE_ALWAYS_CONTINUE(BoidMaterials.IsValidIndex(SpeciesIndex))
		
		for (int i = 0 ; i < DifferentSpeciesBoids[SpeciesIndex].Num(); i++)
		{
			DifferentSpeciesBoids[SpeciesIndex][i]->SetMaterial(BoidMaterials[SpeciesIndex]);
		}
	}
}

void UVisualBoidManagerSubsystem::HandleBoidsUpdate()
{
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	for (int i = 0; i < VisualBoids.Num(); i++)
	{
		FVector NewLocation = BoidManagerSubsystem->GetBoidPositionAt(i);
		FVector NewVelocity = BoidManagerSubsystem->GetBoidVelocityAt(i);
		VisualBoids[i]->UpdateBoid(NewLocation, NewVelocity);
	}
}

void UVisualBoidManagerSubsystem::HandleBoidsUpdateTest()
{
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	for (int SpeciesIndex = 0; SpeciesIndex < DifferentSpeciesBoids.Num(); SpeciesIndex++)
	{
		for (int i = 0 ; i < DifferentSpeciesBoids[SpeciesIndex].Num(); i++)
		{
			FVector NewLocation = BoidManagerSubsystem->GetBoidPositionAtTest(SpeciesIndex, i);
			FVector NewVelocity = BoidManagerSubsystem->GetBoidVelocityAtTest(SpeciesIndex, i);
			DifferentSpeciesBoids[SpeciesIndex][i]->UpdateBoid(NewLocation, NewVelocity);
		}
	}
}

void UVisualBoidManagerSubsystem::HandleBoidsNumberUpdate(FGameplayTag BoidType, int32 NewBoidCount)
{
	
}

void UVisualBoidManagerSubsystem::HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor)
{
	ENSURE_ALWAYS_RETURN(BoidDataManager.IsValid())
	
	const int32 MappedIndexFromGameplayTag = BoidDataManager->RequestMappedIndex(BoidType);
	ENSURE_ALWAYS_RETURN(BoidMaterials.IsValidIndex(MappedIndexFromGameplayTag))
	
	BoidMaterials[MappedIndexFromGameplayTag]->SetVectorParameterValue(FName(FBoidConstants::BoidMaterialColorParameter),
		NewBoidColor);
}

