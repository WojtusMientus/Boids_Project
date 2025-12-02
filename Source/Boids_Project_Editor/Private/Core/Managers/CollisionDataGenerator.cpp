
#include "Core/Managers/CollisionDataGenerator.h"
#include "Utilities/Macros/DebugMacros.h"
#include "DataAssets/BoundsData.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"


TArray<bool> FCollisionDataGenerator::GenerateCollisionData(const FCollisionBoundsPlainData& CollisionBoundsPlainData)
{
	SetupStartingValues(CollisionBoundsPlainData.BoundsPlainData.Center, 
		CollisionBoundsPlainData.BoundsPlainData.Extent,
		CollisionBoundsPlainData.BoundsPlainData.GridResolution);
	
	int32 VoxelGridCellCount = GridResolution.X * GridResolution.Y * GridResolution.Z;

	TArray<bool> ResultedCollisionData;
	ResultedCollisionData.Reserve(VoxelGridCellCount);
	
	UWorld* CurrentWorld = GetCurrentEditorWorld();
	ENSURE_ALWAYS_MESSAGE_RETURN_VALUE(CurrentWorld != nullptr, 
		"Couldn't find world for querying collision data.",
		ResultedCollisionData);
	
	CalculateCollisionData(CurrentWorld, ResultedCollisionData);
	return ResultedCollisionData;
}

void FCollisionDataGenerator::SetupStartingValues(const FVector& Center, const FVector& Extent, const FIntVector Resolution)
{
	GridResolution = Resolution;
	VoxelCellSize = FBoundsMath::GetVoxelCellSize(Center, Extent, GridResolution);
	StartingCellCenter = GetStartingCellCenter(Center, Extent);
}

FVector FCollisionDataGenerator::GetStartingCellCenter(const FVector& BoundsCenter, const FVector& Extent)
{
	const FVector HalfExtentOfBounds = Extent / 2.0f;
	const FVector MinCorner = BoundsCenter - HalfExtentOfBounds;
	
	const float StartingCellCenterX = GetStartingCellCenterAxis(MinCorner.X, VoxelCellSize.X);
	const float StartingCellCenterY = GetStartingCellCenterAxis(MinCorner.Y, VoxelCellSize.Y);
	const float StartingCellCenterZ = GetStartingCellCenterAxis(MinCorner.Z, VoxelCellSize.Z);
		
	return FVector(StartingCellCenterX, StartingCellCenterY, StartingCellCenterZ);
}

float FCollisionDataGenerator::GetStartingCellCenterAxis(const float MinAxis, const float CellAxisSize)
{
	return MinAxis + CellAxisSize / 2.0f;
}

UWorld* FCollisionDataGenerator::GetCurrentEditorWorld()
{
	ENSURE_ALWAYS_MESSAGE_RETURN_VALUE(GEditor != nullptr, "Couldn't find GEditor", nullptr);
	return GEditor->GetEditorWorldContext().World();
}

void FCollisionDataGenerator::CalculateCollisionData(const UWorld* World, TArray<bool>& OutCollisionData)
{
	const FQuat Rotation = FRotator::ZeroRotator.Quaternion();
	constexpr ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldStatic;
	const FCollisionShape CollisionShape = FCollisionShape::MakeBox(VoxelCellSize / 2);
	
	for (int IndexX = 0; IndexX < GridResolution.X; IndexX++)
	{
		for (int IndexY = 0; IndexY < GridResolution.Y; IndexY++)
		{
			for (int IndexZ = 0; IndexZ < GridResolution.Z; IndexZ++)
			{
				const FVector CellCenter = FBoundsMath::GetVoxelCenterAt(StartingCellCenter, VoxelCellSize, 
					IndexX, IndexY, IndexZ);
				
				bool bDidHit = World->OverlapAnyTestByChannel(CellCenter,
					Rotation, TraceChannel, CollisionShape);
				OutCollisionData.Add(bDidHit);
			}
		}
	}
}
