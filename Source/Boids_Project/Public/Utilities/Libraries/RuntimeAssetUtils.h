// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"


/**
 * Runtime asset helper. Used in URuntimeDataLoaderSubsystem and Editor Module.
 */
struct BOIDS_PROJECT_API FRuntimeAssetUtils
{
	static FARFilter GetFilterByClassAndPath(const UClass* ClassToFilter, const FString& PathToFilter, 
	bool bRecursivePath);
	
	/** Retrieves Asset Data from certain path, of certain class.  */
	static TArray<FAssetData> GetFilteredAssetDataByClassAndPath(const UClass* ClassToFilter, 
	const FString& PathToFilter, bool bRecursivePath);
};
