// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BoidSelection.generated.h"

USTRUCT(BlueprintType)
struct FBoidSelection 
{
	GENERATED_BODY()

	FBoidSelection()
	{
	}
	
	FBoidSelection(const int32 Index, const EBoidType BoidType): ID(Index), Type(BoidType)
	{
	}
	
	UPROPERTY(BlueprintReadOnly)
	int32 ID = -1;
	
	UPROPERTY(BlueprintReadOnly)
	EBoidType Type;
	
};
