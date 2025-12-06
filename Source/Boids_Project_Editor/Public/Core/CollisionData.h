#pragma once

#include "CoreMinimal.h"
#include "DataAssets/BoundsData.h"
#include "CollisionData.generated.h"


USTRUCT()
struct FCollisionData
{
	GENERATED_BODY()
	
	FCollisionBoundsPlainData CollisionBoundsData;
	
	TArray<FVector> WallCollisionCentersData;
	
	TArray<FVector> EveryVoxelCenterData;
	
	TArray<FVector> CollisionForcesData;
};