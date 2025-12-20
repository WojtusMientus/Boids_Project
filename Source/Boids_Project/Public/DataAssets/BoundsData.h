// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Bounds/VoxelGrid/VoxelGridData/EnvironmentCollisionCellData.h"
#include "Bounds/VoxelGrid/VoxelGridData/VoxelGridData.h"
#include "BoundsData.generated.h"


struct FCollisionBoundsPlainInfo;


/**
 * Data asset defining parameters for simulation bounds.
 * Configures bounds center, its extent, collision multiplier and precomputed collision forces.
 */
// TODO: Load saved data at simulation start
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
	
	/** Voxelized grid resolution. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "512"))
	FIntVector GridResolution = FIntVector(50,50,50);
	
	/** Number of collision voxel rows around static environment.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "128"))
	int32 EnvironmentCollisionRows = 1;
	
	/** Number of collision voxel rows around the boundaries.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "128"))
	int32 BoundsCollisionRows = 1;
		
	/** Final multiplier applied to environment collision force before retrieving data. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "256"))
	float EnvironmentCollisionMultiplier = 1.0f;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "256"))
	float BoundsCollisionMultiplier = 1.0f;
	
	/** Helper size fo the internal CollisionForces array - overwritten only in OverwriteData function. 
	 *	Made only so unreal doesn't need to render thousands or millions of array entries after opening the asset view.
	 */
	UPROPERTY(VisibleDefaultsOnly)
	FIntVector CollisionArraySize = FIntVector(0,0,0);
	
private:
	
	/** Stored calculated collision forces. */
	TArray<FEnvironmentCollisionCellData> CollisionForces;


public:
	
	TArray<FEnvironmentCollisionCellData> GetCollisionForcesArray() const;
	
	void OverwritePlainData(const FCollisionBoundsPlainInfo& NewCollisionBoundsData,
		const TArray<FEnvironmentCollisionCellData>& NewCollisionWallData);
	
	FCollisionBoundsPlainInfo GetPlainDataInfo() const;
	FEnvironmentCollisionVoxelGridData GetVoxelGridData() const;
};

