
#include "Core/Managers/CollisionDataGenerator.h"
#include "Utilities/Macros/DebugMacros.h"
#include "DataAssets/BoundsData.h"
#include "Utilities/Libraries/BoundsMathLibrary.h"
#include "Core/CollisionData.h"


void FCollisionDataGenerator::GenerateCollisionData(FCollisionData& CollisionData)
{
	SetupStartingValues(CollisionData.CollisionBoundsData);
	
	CollisionData.EveryVoxelCenterData.Empty();
	CollisionData.EveryVoxelCenterData.Reserve(VoxelGridCellCount);
	CollisionData.CollisionForcesData.Empty();
	CollisionData.CollisionForcesData.SetNumZeroed(VoxelGridCellCount);
	
	UWorld* CurrentWorld = GetCurrentEditorWorld();
	ENSURE_ALWAYS_MESSAGE_RETURN(CurrentWorld != nullptr, 
		"Couldn't find world for querying collision data.");
	
	ReserveEstimatedDataSize(VoxelGridCellCount, CollisionData);
	CalculateWallCollisionDataLocations(CurrentWorld, CollisionData);
	CalculateCollisionForces(CollisionData.CollisionForcesData);
}

void FCollisionDataGenerator::SetupStartingValues(const FCollisionBoundsPlainData& CollisionBoundsData)
{
	const FVector BoundsCenter = CollisionBoundsData.BoundsPlainData.Center;
	const FVector BoundsExtent = CollisionBoundsData.BoundsPlainData.Extent;
	
	GridResolution = CollisionBoundsData.BoundsPlainData.GridResolution;
	VoxelCellSize = FBoundsMath::GetVoxelCellSize(BoundsCenter, BoundsExtent, GridResolution);
	StartingCellCenter = GetStartingCellCenter(BoundsCenter, BoundsExtent);
	VoxelGridCellCount = GridResolution.X * GridResolution.Y * GridResolution.Z;
	
	MaxCollisionBoundsDistanceVector = FIntVector(CollisionBoundsData.EnvironmentCollisionRows);
	
	EnvironmentCollisionRows = CollisionBoundsData.EnvironmentCollisionRows;
	BoundsCollisionRows = CollisionBoundsData.BoundsCollisionRows;
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

void FCollisionDataGenerator::ReserveEstimatedDataSize(const int32 NumberOfCells, FCollisionData& VisualizerData)
{
	VisualizerData.WallCollisionCentersData.Empty();
	float OccupiedStoragePercentageEstimate = CalculateEstimatedDataSize(GridResolution.GetMin());
	const int32 OccupiedStorageCount = NumberOfCells * OccupiedStoragePercentageEstimate;
	
	VisualizerData.WallCollisionCentersData.Reserve(OccupiedStorageCount);
}

float FCollisionDataGenerator::CalculateEstimatedDataSize(const int32 GridResolutionMin)
{
	if (GridResolutionMin < 10)
	{
		return 0.5f + 1.0f / 2.0f * GridResolutionMin;
	}
	return 5.0f / GridResolutionMin;
}

void FCollisionDataGenerator::CalculateWallCollisionDataLocations(const UWorld* World, FCollisionData& VisualizerData)
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
				VisualizerData.EveryVoxelCenterData.Add(CellCenter);
				
				if (World->OverlapAnyTestByChannel(CellCenter, Rotation, TraceChannel, CollisionShape))
				{
					VisualizerData.WallCollisionCentersData.Add(CellCenter);
					WallCollisionDataIndices.Add(FIntVector(IndexX, IndexY, IndexZ));
				}
			}
		}
	}
}


void FCollisionDataGenerator::CalculateCollisionForces(TArray<FVector>& OutCollisionForces)
{
	for (const FIntVector CollidedWall : WallCollisionDataIndices)
	{
		AddCollisionToNeighborCells(CollidedWall, OutCollisionForces);
	}
	
	WallCollisionDataIndices.Empty();
}

void FCollisionDataGenerator::AddCollisionToNeighborCells(const FIntVector& WallCell, TArray<FVector>& OutCollisionForces)
{
	const FIntVector StartingIndices = FBoundsMath::ClampToBoundsResolution(WallCell - MaxCollisionBoundsDistanceVector,
		GridResolution);
	const FIntVector EndingIndices = FBoundsMath::ClampToBoundsResolution(WallCell + MaxCollisionBoundsDistanceVector,
		GridResolution);
	
	for (int IndexX = StartingIndices.X; IndexX <= EndingIndices.X; IndexX++)
	{
		for (int IndexY = StartingIndices.Y; IndexY <= EndingIndices.Y; IndexY++)
		{
			for (int IndexZ = StartingIndices.Z; IndexZ <= EndingIndices.Z; IndexZ++)
			{
				const FIntVector CurrentCell = FIntVector(IndexX, IndexY, IndexZ);
				if (WallCollisionDataIndices.Contains(CurrentCell))
				{
					continue;
				}
				
				const FVector CollisionForce = CalculateCollisionForceAt(FIntVector(IndexX, IndexY, IndexZ), WallCell);
				const int32 ArrayIndex = FBoundsMath::XYZToArrayIndex(CurrentCell, GridResolution);
				OutCollisionForces[ArrayIndex] += CollisionForce;
			}
		}
	}
}

FVector FCollisionDataGenerator::CalculateCollisionForceAt(const FIntVector& CurrentVoxel, const FIntVector& StartingVoxel)
{
	const float DistanceBetweenCells = FBoundsMath::GetMaxDistanceBetweenVoxelCells(CurrentVoxel, StartingVoxel);
	FVector DirectionToCell = FVector(CurrentVoxel - StartingVoxel).GetSafeNormal();
	return DirectionToCell * MaxCollisionBoundsDistance / DistanceBetweenCells;
}
