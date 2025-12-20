
#include "Core/Subsystems/VisualBoidManagerSubsystem.h"
#include "Utilities/Subsystems/RuntimeDataLoaderSubsystem.h"
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

	UGameInstance* GameInstance = World->GetGameInstance();
	ENSURE_ALWAYS_RETURN(IsValid(GameInstance))
	
	BoidManagerSubsystem = GameInstance->GetSubsystem<UBoidManagerSubsystem>();
	ENSURE_ALWAYS_RETURN(BoidManagerSubsystem.IsValid())
	
	RuntimeDataLoaderSubsystem = GameInstance->GetSubsystem<URuntimeDataLoaderSubsystem>();
	
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

void UVisualBoidManagerSubsystem::PostWorldInitialized(UWorld* World,
	FWorldInitializationValues WorldInitializationValues)
{
	InitializeBoids();
	RequestMaterialInstanceAsset();
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
		&UVisualBoidManagerSubsystem::HandleBoidsUpdate);
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

void UVisualBoidManagerSubsystem::RequestMaterialInstanceAsset()
{
	ENSURE_ALWAYS_RETURN(RuntimeDataLoaderSubsystem.IsValid())
	
	TArray<FLoadRequest> LoadRequests = URuntimeDataLoaderSubsystem::CreateLoadRequest(
		EAssetRequestType::SingleAsset, FBoidConstants::Paths::MaterialsPath, 
		UMaterialInstanceConstant::StaticClass());
	
	RuntimeDataLoaderSubsystem->LoadAssets(LoadRequests, FOnBatchLoadCompleteEvent::CreateUObject(this, 
		&UVisualBoidManagerSubsystem::HandleLoadedMaterialAsset));
}

void UVisualBoidManagerSubsystem::HandleLoadedMaterialAsset(const TArray<FLoadedGroup>& LoadedAssets)
{
	if (LoadedAssets.IsEmpty() || LoadedAssets[0].LoadedAssets.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Loaded Assets"));
		return;
	}
	if (UMaterialInstanceConstant* LoadedMaterialInstance = Cast<UMaterialInstanceConstant>(LoadedAssets[0].LoadedAssets[0]))
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(LoadedMaterialInstance, this);
		ENSURE_ALWAYS_RETURN(IsValid(DynamicMaterialInstance))
		DynamicMaterialInstance->SetVectorParameterValue(FName(FBoidConstants::BoidMaterialColorParameter), 
			FLinearColor::Red);
	}	
	
	for (int i = 0; i < VisualBoids.Num(); i++)
	{
		VisualBoids[i]->SetMaterial(DynamicMaterialInstance);
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

void UVisualBoidManagerSubsystem::HandleBoidsNumberUpdate(FGameplayTag BoidType, int32 NewBoidCount)
{
	
}

void UVisualBoidManagerSubsystem::HandleBoidsColorUpdate(FGameplayTag BoidType, FLinearColor NewBoidColor)
{
	ENSURE_ALWAYS_RETURN(IsValid(DynamicMaterialInstance))
	DynamicMaterialInstance->SetVectorParameterValue(FName(FBoidConstants::BoidMaterialColorParameter), NewBoidColor);
}

