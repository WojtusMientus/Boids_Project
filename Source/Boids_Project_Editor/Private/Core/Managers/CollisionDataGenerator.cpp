
#include "Core/Managers/CollisionDataGenerator.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Bounds/VoxelGrid/VoxelGridData/EnvironmentCollisionCellData.h"
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
	ENSURE_ALWAYS_RETURN(IsValid(CurrentWorld));
	
	ReserveEstimatedDataSize(VoxelGridCellCount, CollisionData);
	CalculateWallCollisionDataLocations(CurrentWorld, CollisionData);
	CalculateCollisionForces(CollisionData.CollisionForcesData);
}

void FCollisionDataGenerator::SetupStartingValues(const FCollisionBoundsPlainInfo& CollisionBoundsData)
{
	const FVector BoundsCenter = CollisionBoundsData.BoundsPlainData.Center;
	const FVector BoundsExtent = CollisionBoundsData.BoundsPlainData.Extent;
	
	GridResolution = CollisionBoundsData.BoundsPlainData.GridResolution;
	VoxelCellSize = FBoundsMath::GetVoxelCellSize(BoundsCenter, BoundsExtent, GridResolution);
	StartingCellCenter = FBoundsMath::GetStartingCellCenter(BoundsCenter, BoundsExtent, GridResolution);
	VoxelGridCellCount = GridResolution.X * GridResolution.Y * GridResolution.Z;
	
	MaxCollisionBoundsDistanceVector = FIntVector(CollisionBoundsData.EnvironmentCollisionRows);
	BoundsCollisionRows = CollisionBoundsData.BoundsCollisionRows;
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


void FCollisionDataGenerator::CalculateCollisionForces(TArray<FEnvironmentCollisionCellData>& OutCollisionForces)
{
	for (const FIntVector CollidedWall : WallCollisionDataIndices)
	{
		AddWallCollisionForceToNeighborCells(CollidedWall, OutCollisionForces);
	}
	CalculateBoundsCollisionForces(OutCollisionForces);
	WallCollisionDataIndices.Empty();
}

void FCollisionDataGenerator::AddWallCollisionForceToNeighborCells(const FIntVector& WallCell, 
	TArray<FEnvironmentCollisionCellData>& OutCollisionForces)
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
				
				const FVector CollisionForce = CalculateWallCollisionForceAt(
					FIntVector(IndexX, IndexY, IndexZ), WallCell);
				const int32 ArrayIndex = FBoundsMath::XYZToArrayIndex(CurrentCell, GridResolution);
				OutCollisionForces[ArrayIndex].EnvironmentCollisionForce += CollisionForce;
			}
		}
	}
}

FVector FCollisionDataGenerator::CalculateWallCollisionForceAt(const FIntVector& CurrentVoxel, const FIntVector& StartingVoxel)
{
	const float DistanceBetweenCells = FBoundsMath::GetMaxDistanceBetweenVoxelCells(CurrentVoxel, StartingVoxel);
	FVector DirectionToCell = FVector(CurrentVoxel - StartingVoxel).GetSafeNormal();
	return DirectionToCell * MaxCollisionBoundsDistance / DistanceBetweenCells;
}

void FCollisionDataGenerator::CalculateBoundsCollisionForces(TArray<FEnvironmentCollisionCellData>& OutCollisionForces)
{
	CalculateForcesAlongX(0, BoundsCollisionRows, FVector::ForwardVector, 
		true, OutCollisionForces);
	CalculateForcesAlongX(GridResolution.X - BoundsCollisionRows, GridResolution.X, 
		FVector::BackwardVector, false, OutCollisionForces);
	
	CalculateForcesAlongY(0, BoundsCollisionRows, FVector::RightVector, 
		true, OutCollisionForces);
	CalculateForcesAlongY(GridResolution.Y - BoundsCollisionRows, GridResolution.Y, 
		FVector::LeftVector, false, OutCollisionForces);
	
	CalculateForcesAlongZ(0, BoundsCollisionRows, FVector::UpVector, 
		true, OutCollisionForces);
	CalculateForcesAlongZ(GridResolution.Z - BoundsCollisionRows, GridResolution.Z, 
		FVector::DownVector, false, OutCollisionForces);
}

void FCollisionDataGenerator::CalculateForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary, TArray<FEnvironmentCollisionCellData>& OutCollisionForces)
{
	const int32 ClampedStartIndex = FMath::Clamp(StartIndex, 0, GridResolution.X);
	const int32 ClampedEndIndex = FMath::Clamp(EndIndex, 0, GridResolution.X);
	
	for (int IndexX = ClampedStartIndex; IndexX < ClampedEndIndex; IndexX++)
	{
		int ForceMagnitude = IndexX - ClampedStartIndex + BoundsCollisionRows;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = BoundsCollisionRows - IndexX;
		}
		
		for (int IndexY = 0; IndexY < GridResolution.Y; IndexY++)
		{
			for (int IndexZ = 0; IndexZ < GridResolution.Z; IndexZ++)
			{
				const int32 ArrayIndex = FBoundsMath::XYZToArrayIndex(IndexX, IndexY, IndexZ, GridResolution);
				OutCollisionForces[ArrayIndex].BoundsCollisionForce += ForceVector * ForceMagnitude;
			}
		}
	}
}

void FCollisionDataGenerator::CalculateForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary, TArray<FEnvironmentCollisionCellData>& OutCollisionForces)
{
	const int32 ClampedStartIndex = FMath::Clamp(StartIndex, 0, GridResolution.Y);
	const int32 ClampedEndIndex = FMath::Clamp(EndIndex, 0, GridResolution.Y);
	
	for (int IndexY = ClampedStartIndex; IndexY < ClampedEndIndex; IndexY++)
	{
		int ForceMagnitude = GridResolution.Y - IndexY;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = BoundsCollisionRows - IndexY;
		}
		
		for (int IndexX = 0; IndexX < GridResolution.X; IndexX++)
		{
			for (int IndexZ = 0; IndexZ < GridResolution.Z; IndexZ++)
			{
				const int32 ArrayIndex = FBoundsMath::XYZToArrayIndex(IndexX, IndexY, IndexZ, GridResolution);
				OutCollisionForces[ArrayIndex].BoundsCollisionForce += ForceVector * ForceMagnitude;
			}
		}
	}
}

void FCollisionDataGenerator::CalculateForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary, TArray<FEnvironmentCollisionCellData>& OutCollisionForces)
{
	const int32 ClampedStartIndex = FMath::Clamp(StartIndex, 0, GridResolution.Z);
	const int32 ClampedEndIndex = FMath::Clamp(EndIndex, 0, GridResolution.Z);
	
	for (int IndexZ = ClampedStartIndex; IndexZ < ClampedEndIndex; IndexZ++)
	{
		int ForceMagnitude = GridResolution.Z - IndexZ;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = BoundsCollisionRows - IndexZ;
		}
		
		for (int IndexX = 0; IndexX < GridResolution.X; IndexX++)
		{
			for (int IndexY = 0; IndexY < GridResolution.Y; IndexY++)
			{
				const int32 ArrayIndex = FBoundsMath::XYZToArrayIndex(IndexX, IndexY, IndexZ, GridResolution);
				OutCollisionForces[ArrayIndex].BoundsCollisionForce += ForceVector * ForceMagnitude;
			}
		}
	}
}
