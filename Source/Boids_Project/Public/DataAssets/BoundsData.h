// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoundsData.generated.h"


/**
 * Helper struct for passing data between EditorBoidDataManager and its delegate listeners.
 */
USTRUCT(BlueprintType)
struct FBoundsPlainData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Center = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FVector Extent = FVector(100.0f,100.0f,100.0f);

	UPROPERTY(BlueprintReadWrite)
	FIntVector GridResolution = FIntVector(1,1,1);
};

/**
 * Helper struct for passing data between tool and EditorBoidDataManager for saving and simplicity purposes.
 */
USTRUCT(BlueprintType)
struct FCollisionBoundsPlainData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	FBoundsPlainData BoundsPlainData;
	
	UPROPERTY(BlueprintReadWrite)
	int32 EnvironmentCollisionRows = 1;
	
	UPROPERTY(BlueprintReadWrite)
	int32 BoundsCollisionRows = 1;
	
	UPROPERTY(BlueprintReadWrite)
	float EnvironmentCollisionMultiplier = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float BoundsCollisionMultiplier = 1.0f;
	
	
	void OverwriteData(const UBoundsData* NewBoundsData);
};


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
	FVector Extent = FVector(100.0f, 100.0f, 100.0f);
	
	/** Voxelized grid resolution. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "512"))
	FIntVector GridResolution = FIntVector(1,1,1);
	
	/** Number of collision voxel rows around static environment.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "20"))
	int32 EnvironmentCollisionRows = 1;
	
	/** Number of collision voxel rows around the boundaries.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "20"))
	int32 BoundsCollisionRows = 1;
		
	/** Final multiplier applied to environment collision force before retrieving data. */
	float EnvironmentCollisionMultiplier = 1.0f;
	
	/** Final multiplier applied to bounds collision force before retrieving data. */
	float BoundsCollisionMultiplier = 1.0f;
	
	/** Helper size fo the internal CollisionForces array - overwritten only in OverwriteData function. 
	 *	Made only so unreal doesn't need to render thousands or millions of array entries after opening the asset view.
	 */
	UPROPERTY(VisibleDefaultsOnly)
	FIntVector CollisionArraySize = FIntVector(0,0,0);
	
private:
	
	/** Stored calculated collision forces. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<FVector> CollisionForces;

public:
	
	void OverwriteData(const FCollisionBoundsPlainData& NewCollisionBoundsData,
		const TArray<FVector>& NewCollisionWallData)
	{
		Center = NewCollisionBoundsData.BoundsPlainData.Center;
		Extent = NewCollisionBoundsData.BoundsPlainData.Extent;
		GridResolution = NewCollisionBoundsData.BoundsPlainData.GridResolution;
		EnvironmentCollisionRows = NewCollisionBoundsData.EnvironmentCollisionRows;
		BoundsCollisionMultiplier = NewCollisionBoundsData.BoundsCollisionMultiplier;
		CollisionForces = NewCollisionWallData;
		CollisionArraySize = GridResolution;
	}
};

inline void FCollisionBoundsPlainData::OverwriteData(const UBoundsData* NewBoundsData)
{
	if (!NewBoundsData)
	{
		return;
	}
	
	BoundsPlainData.Center = NewBoundsData->Center;
	BoundsPlainData.Extent = NewBoundsData->Extent;
	BoundsPlainData.GridResolution = NewBoundsData->GridResolution;
	EnvironmentCollisionRows = NewBoundsData->EnvironmentCollisionRows;
	BoundsCollisionRows = NewBoundsData->BoundsCollisionRows;
	EnvironmentCollisionMultiplier = NewBoundsData->EnvironmentCollisionMultiplier;
	BoundsCollisionMultiplier = NewBoundsData->BoundsCollisionMultiplier;
}

