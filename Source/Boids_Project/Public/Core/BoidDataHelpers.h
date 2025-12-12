
#pragma once

#include "CoreMinimal.h"


namespace BoidDataHelpers
{
	BOIDS_PROJECT_API FString GetBoidSpeciesTagName();
	BOIDS_PROJECT_API FString GetBoidsSpeciesDataDirectoryName();
	BOIDS_PROJECT_API FString GetBoidsBoundsDirectoryName();
	BOIDS_PROJECT_API FString GetBoidsMaterialsDirectoryName();
	
	BOIDS_PROJECT_API FARFilter GetFilterByClassAndPath(const UClass* ClassToFilter, const FString& PathToFilter);
}
