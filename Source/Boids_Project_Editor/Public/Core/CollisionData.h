#pragma once

#include "CoreMinimal.h"
#include "Bounds/VoxelGrid/VoxelGridData/VoxelGridData.h"
#include "CollisionData.generated.h"


USTRUCT()
struct FCollisionData
{
	GENERATED_BODY()
	
	FCollisionBoundsPlainInfo CollisionBoundsData;
	
	TArray<FVector> WallCollisionCentersData;
	
	TArray<FVector> EveryVoxelCenterData;
	
	TArray<FEnvironmentCollisionCellData> CollisionForcesData;
};