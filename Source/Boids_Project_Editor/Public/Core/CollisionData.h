// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "VoxelGrids/VoxelGridData/VoxelGridData.h"
#include "CollisionData.generated.h"


/** Helper struct for passing data between UBoidEditorSubsystem and its delegate listeners. */
USTRUCT()
struct FCollisionData
{
	GENERATED_BODY()
	
	FCollisionBoundsPlainInfo CollisionBoundsData;
	
	TArray<FEnvironmentCollisionCellData> CollisionForcesData;
};