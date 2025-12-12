#include "Bounds/VoxelGrid/BoidCollisionVoxelGrid.h"

FBoidCollisionVoxelGrid::FBoidCollisionVoxelGrid(const FVoxelGridData<TArray<int32>>& VoxelGridData)
	: FVoxelGrid<TArray<int32>>(VoxelGridData)
{
}

void FBoidCollisionVoxelGrid::InitializeBoidCollisionGrid(const int32 StartingNumberOfBoids)
{
	FVoxelGrid<TArray<int32>>::InitializeVoxelGrid();
	
	for (TArray<int32>& VoxelCellArray: InternalVoxelGrid)
	{
		VoxelCellArray.Reserve(StartingNumberOfBoids / 20);
	}
}

void FBoidCollisionVoxelGrid::AddBoidToVoxelGrid(const int32 BoidID, const FVector& BoidPosition)
{
	const int32 GridIndex = PositionToArrayIndex(BoidPosition);
	AddBoidToVoxelGridInternal(BoidID, GridIndex);
}

void FBoidCollisionVoxelGrid::UpdateBoid(const FVector& BoidPosition, const int32 GridIndex, const int32 GridCellIndex)
{
#if WITH_EDITOR
	ENSURE_ARRAY_INDEX_VALID(GridIndex)
	ENSURE_ARRAY_CELL_INDEX_VALID(GridIndex, GridCellIndex)
#endif
	
	const int32 NewGridIndex = PositionToArrayIndex(BoidPosition);
	
	if (NewGridIndex == GridIndex)
	{
		return;
	}
	
	TrySwapBoidToLastIndex(GridIndex, GridCellIndex);
	const int32 PoppedID = InternalVoxelGrid[GridIndex].Pop(EAllowShrinking::No);
	AddBoidToVoxelGridInternal(PoppedID, NewGridIndex);
}

void FBoidCollisionVoxelGrid::RemoveBoid(const int32 GridIndex, const int32 GridCellIndex)
{
	TrySwapBoidToLastIndex(GridIndex, GridCellIndex);
	const int32 RemovedID = InternalVoxelGrid[GridIndex].Pop(EAllowShrinking::No);
	OnBoidGridIndexChanged.Broadcast(RemovedID, INDEX_NONE, INDEX_NONE);
}

void FBoidCollisionVoxelGrid::TrySwapBoidToLastIndex(const int32 GridIndex, const int32 GridCellIndex)
{
	const int32 LastViableIndexInCurrentCell = InternalVoxelGrid[GridIndex].Num() - 1;
	
	if (LastViableIndexInCurrentCell != GridCellIndex)
	{
		const int32 SwappedBoidID = InternalVoxelGrid[GridIndex][LastViableIndexInCurrentCell];
		InternalVoxelGrid[GridIndex][LastViableIndexInCurrentCell] = InternalVoxelGrid[GridIndex][GridCellIndex];
		InternalVoxelGrid[GridIndex][GridCellIndex] = SwappedBoidID;
		
		OnBoidGridIndexChanged.Broadcast(SwappedBoidID, GridIndex, GridCellIndex);
	}
}

void FBoidCollisionVoxelGrid::AddBoidToVoxelGridInternal(const int32 BoidID, const int32 NewGridIndex)
{
	const int32 GridCellIndex = InternalVoxelGrid[NewGridIndex].Add(BoidID);
	OnBoidGridIndexChanged.Broadcast(BoidID, NewGridIndex, GridCellIndex);
}

