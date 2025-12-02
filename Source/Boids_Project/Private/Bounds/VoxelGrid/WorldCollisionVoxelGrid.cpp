#include "Bounds/VoxelGrid/WorldCollisionVoxelGrid.h"


FWorldCollisionVoxelGrid::FWorldCollisionVoxelGrid(const FVoxelGridCollisionData& VoxelGridCollisionData): 
FVoxelGrid<FEnvironmentCollisionData>(VoxelGridCollisionData.VoxelGridData),
BoundsCollisionMultiplier(VoxelGridCollisionData.BoundsCollisionMultiplier),
EnvironmentCollisionMultiplier(VoxelGridCollisionData.EnvironmentCollisionMultiplier)
{
}

void FWorldCollisionVoxelGrid::InitializeVoxelGrid()
{
	FVoxelGrid<FEnvironmentCollisionData>::InitializeVoxelGrid();
	InitializeCollisionGrid();
}

FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtIndex(const int32 Index) const
{
	const FEnvironmentCollisionData CollisionData = FVoxelGrid<FEnvironmentCollisionData>::GetValueAtIndex(Index);
	return CollisionData.EnvironmentCollisionForce * EnvironmentCollisionMultiplier + 
		CollisionData.BoundsCollisionForce * BoundsCollisionMultiplier;
}

FVector FWorldCollisionVoxelGrid::GetFinalCollisionVectorAtLocation(const FVector& Location) const
{
	const FEnvironmentCollisionData CollisionData = FVoxelGrid<FEnvironmentCollisionData>::GetValueAtLocation(Location);
	return CollisionData.EnvironmentCollisionForce * EnvironmentCollisionMultiplier + 
		CollisionData.BoundsCollisionForce * BoundsCollisionMultiplier;
}

void FWorldCollisionVoxelGrid::UpdateCollisionMultiplier(float NewCollisionMultiplier)
{
	CollisionMultiplier = NewCollisionMultiplier;
}

void FWorldCollisionVoxelGrid::InitializeCollisionGrid()
{
	InitializeForcesAlongX(0, COLLISION_ROWS, FVector::ForwardVector, true);
	InitializeForcesAlongX(DimensionX - COLLISION_ROWS, DimensionX, FVector::BackwardVector, false);
	
	InitializeForcesAlongY(0, COLLISION_ROWS, FVector::RightVector, true);
	InitializeForcesAlongY(DimensionY - COLLISION_ROWS, DimensionY, FVector::LeftVector, false);
	
	InitializeForcesAlongZ(0, COLLISION_ROWS, FVector::UpVector, true);
	InitializeForcesAlongZ(DimensionZ - COLLISION_ROWS, DimensionZ, FVector::DownVector, false);
}

void FWorldCollisionVoxelGrid::InitializeForcesAlongX(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary)
{
	for (int x = StartIndex; x < EndIndex; x++)
	{
		int ForceMagnitude = DimensionX - x;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = COLLISION_ROWS - x;
		}
		
		for (int y = 0; y < DimensionY; y++)
		{
			for (int z = 0; z < DimensionZ; z++)
			{
				const int32 Index = PositionToArrayIndex(x, y, z);
				AddForceAt(ForceVector * ForceMagnitude, Index);
			}
		}
	}
}

void FWorldCollisionVoxelGrid::InitializeForcesAlongY(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary)
{
	for (int y = StartIndex; y < EndIndex; y++)
	{
		int ForceMagnitude = DimensionY - y;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = COLLISION_ROWS - y;
		}
		
		for (int x = 0; x < DimensionX; x++)
		{
			for (int z = 0; z < DimensionZ; z++)
			{
				const int32 Index = PositionToArrayIndex(x, y, z);
				AddForceAt(ForceVector * ForceMagnitude, Index);
			}
		}
	}
}

void FWorldCollisionVoxelGrid::InitializeForcesAlongZ(int StartIndex, int EndIndex, const FVector& ForceVector,
	bool bAtLowerBoundary)
{
	for (int z = StartIndex; z < EndIndex; z++)
	{
		int ForceMagnitude = DimensionZ - z;
		
		if (bAtLowerBoundary)
		{
			ForceMagnitude = COLLISION_ROWS - z;
		}
		
		for (int x = 0; x < DimensionX; x++)
		{
			for (int y = 0; y < DimensionY; y++)
			{
				const int32 Index = PositionToArrayIndex(x, y, z);
				AddForceAt(ForceVector * ForceMagnitude, Index);
			}
		}
	}
}

void FWorldCollisionVoxelGrid::AddForceAt(const FVector& Force, int32 Index)
{
	// InternalVoxelGrid[Index] += Force;
}
