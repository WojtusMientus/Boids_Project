// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Boids_Project/Globals.h"
#include "BoidSelectionInfo.generated.h"

/**
 * Holds information about a Boid selected in the editor.
 */
USTRUCT(BlueprintType)
struct FBoidSelectionInfo 
{
	GENERATED_BODY()
	
	FBoidSelectionInfo() = default;

	/**
	 * Constructs BoidSelectionInfo with unique ID and its type.
	 * @param ID Unique ID of the selected Boid.
	 * @param BoidType Species type of the Boid.
	 */
	FBoidSelectionInfo(const int32 ID, const EBoidType BoidType): ID(ID), Type(BoidType)
	{
	}

	/** Unique ID of the selected Boid. */
	UPROPERTY(BlueprintReadOnly)
	int32 ID = -1;

	/** Species type of the Boid. */
	UPROPERTY(BlueprintReadOnly)
	EBoidType Type;
	
};
