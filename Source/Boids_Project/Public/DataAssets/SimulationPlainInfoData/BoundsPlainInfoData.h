// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "BoundsPlainInfoData.generated.h"


class UBoundsData;


/**
 * Helper struct for passing data between EditorBoidDataManager and its delegate listeners.
 */
USTRUCT(BlueprintType)
struct BOIDS_PROJECT_API FBoundsPlainInfo
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	FVector Center = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FVector Extent = FVector(1000.0f,1000.0f,1000.0f);

	UPROPERTY(BlueprintReadWrite)
	FIntVector GridResolution = FIntVector(1,1,1);
};

/**
 * Helper struct for passing data between tool and EditorBoidDataManager for saving and simplicity purposes.
 */
USTRUCT(BlueprintType)
struct BOIDS_PROJECT_API FCollisionBoundsPlainInfo
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	FBoundsPlainInfo BoundsPlainData;
	
	UPROPERTY(BlueprintReadWrite)
	int32 EnvironmentCollisionRows = 1;
	
	UPROPERTY(BlueprintReadWrite)
	int32 BoundsCollisionRows = 1;
	
	UPROPERTY(BlueprintReadWrite)
	float EnvironmentCollisionMultiplier = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float BoundsCollisionMultiplier = 1.0f;
	
	FCollisionBoundsPlainInfo() {};
	FCollisionBoundsPlainInfo(const UBoundsData* NewBoundsData);
	
	void OverwriteData(const UBoundsData* NewBoundsData);
};