
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Boids_Project/Globals.h"
#include "BoidSelection.generated.h"

USTRUCT(BlueprintType)
struct FBoidSelection 
{
	GENERATED_BODY()

	// ----- Constructors -----
	FBoidSelection() = default;
	FBoidSelection(const int32 Index, const EBoidType BoidType): ID(Index), Type(BoidType)
	{
	}
	
	UPROPERTY(BlueprintReadOnly)
	int32 ID = -1;
	
	UPROPERTY(BlueprintReadOnly)
	EBoidType Type;
	
};
