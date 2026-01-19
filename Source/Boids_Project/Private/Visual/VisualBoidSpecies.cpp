
#include "Visual/VisualBoidSpecies.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Utilities/BoidConstants.h"
#include "Utilities/ObjectPools/VisualBoidPool.h"
#include "Visual/VisualBoid.h"


void UVisualBoidSpecies::InitializeSpecies(const int32 BoidCount, TSubclassOf<AVisualBoid> VisualBoidClass, 
	 UWorld* InSimulationWorld)
{
	VisualBoidPool = NewObject<UVisualBoidPool>();
	check(VisualBoidPool != nullptr);
	VisualBoidPool->InitializeVisualBoidPool(BoidCount, 100, VisualBoidClass, InSimulationWorld);
	VisualBoidPool->AddVisualBoids(BoidCount);
}

void UVisualBoidSpecies::InitializeMaterial(UMaterialInstanceConstant* LoadedMaterialInstance)
{
	BoidMaterial = UMaterialInstanceDynamic::Create(LoadedMaterialInstance, this);
	check(BoidMaterial != nullptr);
	ApplyMaterial(0, Num());
}

void UVisualBoidSpecies::ApplyMaterial(const int32 StartIndex, const int32 EndIndex)
{
	const int32 FinalIndex = FMath::Clamp(EndIndex, 0, VisualBoidPool->Num());
	
	for (int BoidIndex = StartIndex; BoidIndex < FinalIndex; BoidIndex++)
	{
		VisualBoidPool->Get(BoidIndex)->SetMaterial(BoidMaterial);
	}
}

void UVisualBoidSpecies::UpdateMaterialColor(FLinearColor NewBoidColor)
{
	BoidMaterial->SetVectorParameterValue(FName(FBoidConstants::BoidMaterialColorParameter), NewBoidColor);
}

AVisualBoid* UVisualBoidSpecies::Get(const int32 BoidIndex)
{
	return VisualBoidPool->Get(BoidIndex);
}

void UVisualBoidSpecies::AddBoids(const int32 CountToAdd)
{
	VisualBoidPool->AddVisualBoids(CountToAdd);
}

void UVisualBoidSpecies::RemoveBoids(const int32 CountToRemove)
{
	VisualBoidPool->RemoveLastUsed(CountToRemove);
}

int32 UVisualBoidSpecies::Num() const
{
	return VisualBoidPool->Num();
}
