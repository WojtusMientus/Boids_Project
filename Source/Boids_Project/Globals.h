// Copyright WojtusMientus

#pragma once
 
#include "CoreMinimal.h"

/**
 * Defines the type of Boid - same types flock together, while ignoring different ones.
 */
// TODO: Type-based flocking is not yet implemented.
UENUM(BlueprintType)
enum class EBoidType : uint8
{
	EBoid_1 UMETA(DisplayName = "Boid Type 1"),
	EBoid_2 UMETA(DisplayName = "Boid Type 2"),
	EBoid_3 UMETA(DisplayName = "Boid Type 3"),
	EBoid_4 UMETA(DisplayName = "Boid Type 4"),
};