// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VoxelGrids/VoxelGridData/EnvironmentCollisionCellData.h"
#include "VoxelGrids/VoxelGridData/VoxelGridData.h"
#include "BoundsData.generated.h"

struct FCollisionBoundsPlainInfo;


/**
 * Data asset defining parameters for simulation bounds.
 * Configures bounds center, its extent, grid resolutions, count of collision rows and precomputed forces.
 */
UCLASS(BlueprintType)
class BOIDS_PROJECT_API UBoundsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/** World location of the bounds center. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector Center =  FVector::ZeroVector;
	
	/** Extent of the bounds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector Extent = FVector(1000.0f, 1000.0f, 1000.0f);
	
	/** Voxelized environment grid resolution. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "512"))
	FIntVector EnvironmentGridResolution = FIntVector(50,50,50);
	
	/** Voxelized boid collision grid resolution. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "512"))
	FIntVector BoidCollisionGridResolution = FIntVector(50,50,50);
	
	/** Number of collision voxel rows around static environment.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "128"))
	int32 EnvironmentCollisionRows = 1;
	
	/** Number of collision voxel rows around the boundaries.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "128"))
	int32 BoundsCollisionRows = 1;
	
	/** Voxel that is in simulation area for future FloodFill algorithm for proper collision data. (not in a mesh) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "512"))
	FIntVector SimulationSeedVoxelIndices = FIntVector(1,1,1);
	
	
	/** Helper size of the internal CollisionForces array - overwritten only in OverwriteData function. 
	 *	Made only so unreal doesn't need to render thousands or millions of array entries after opening the asset view.
	 *	(still lags a bit)
	 */
	UPROPERTY(VisibleDefaultsOnly)
	FIntVector CollisionArraySize = FIntVector(0,0,0);
	
private:
	
	/** Stored calculated collision forces. */
	UPROPERTY()
	TArray<FEnvironmentCollisionCellData> CollisionForces;


public:
	
	TArray<FEnvironmentCollisionCellData> GetCollisionForcesArray() const;
	
	void OverwritePlainData(const FCollisionBoundsPlainInfo& NewCollisionBoundsData,
		const TArray<FEnvironmentCollisionCellData>& NewCollisionWallData);
};

