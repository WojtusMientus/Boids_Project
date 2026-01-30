
#include "Visual/VisualBoidSpecies.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Utilities/BoidConstants.h"


AVisualBoidSpecies::AVisualBoidSpecies()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InitializeInstancedStaticMeshComponent();
}

void AVisualBoidSpecies::InitializeSpecies(UStaticMesh* BoidMesh, const int32 BoidsCount)
{
	check(IsValid(BoidMesh));
	VisualBoidMeshes->SetStaticMesh(BoidMesh);
	VisualBoidTransforms.AddDefaulted(BoidsCount);
	AddBoidsInternal(VisualBoidTransforms);
}

void AVisualBoidSpecies::InitializeMaterial(UMaterialInstanceConstant* LoadedMaterialInstance)
{
	BoidMaterial = UMaterialInstanceDynamic::Create(LoadedMaterialInstance, this);
	check(BoidMaterial != nullptr);
	ApplyMaterial();
}

void AVisualBoidSpecies::ApplyMaterial()
{
	VisualBoidMeshes->SetMaterial(0, BoidMaterial);
}

void AVisualBoidSpecies::UpdateMaterialColor(FLinearColor NewBoidColor)
{
	BoidMaterial->SetVectorParameterValue(FName(FBoidConstants::BoidMaterialColorParameter), NewBoidColor);
}

void AVisualBoidSpecies::UpdateBoidTransform(const int32 BoidIndex, const FVector& NewPosition,
	const FVector& NewRotation)
{
	VisualBoidTransforms[BoidIndex].SetLocation(NewPosition);
	VisualBoidTransforms[BoidIndex].SetRotation(NewRotation.ToOrientationQuat());
}

void AVisualBoidSpecies::UpdateBoidTransforms()
{
	VisualBoidMeshes->BatchUpdateInstancesTransforms(0, VisualBoidTransforms, true,
		false, false);
}

void AVisualBoidSpecies::AddBoids(const TArray<FTransform>& NewBoidTransforms)
{
	for (int Index = 0; Index < NewBoidTransforms.Num(); Index++)
	{
		VisualBoidTransforms.Add(NewBoidTransforms[Index]);
	}
	
	AddBoidsInternal(NewBoidTransforms);
}

void AVisualBoidSpecies::RemoveBoids(const int32 CountToRemove)
{
	for (int i = 0; i < CountToRemove; i++)
	{
		VisualBoidTransforms.Pop(EAllowShrinking::No);
		VisualBoidMeshes->RemoveInstance(VisualBoidMeshes->GetInstanceCount() - 1);
	}
}

int32 AVisualBoidSpecies::Num() const
{
	return VisualBoidMeshes->GetInstanceCount();
}

void AVisualBoidSpecies::InitializeInstancedStaticMeshComponent()
{
	VisualBoidMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BoidInstancedMesh"));
	check(IsValid(VisualBoidMeshes));
	VisualBoidMeshes->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	VisualBoidMeshes->SetMobility(EComponentMobility::Type::Movable);
	SetRootComponent(VisualBoidMeshes);
}

void AVisualBoidSpecies::AddBoidsInternal(const TArray<FTransform>& NewBoidTransforms)
{
	VisualBoidMeshes->AddInstances(NewBoidTransforms, false, true);
}
