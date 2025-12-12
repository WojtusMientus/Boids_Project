
#include "Core/BoidDataHelpers.h"

namespace BoidDataHelpers
{
	FString GetBoidSpeciesTagName()
	{
		return FString("Boid.Species");
	}

	FString GetBoidsSpeciesDataDirectoryName()
	{
		return FString("/Game/Assets/DataAssets/BoidSpecies");
	}

	FString GetBoidsBoundsDirectoryName()
	{
		return FString("/Game/Assets/DataAssets/Bounds");
	}

	FString GetBoidsMaterialsDirectoryName()
	{
		return FString("/Game/Assets/Materials/Boids");
	}

	FARFilter GetFilterByClassAndPath(const UClass* ClassToFilter, const FString& PathToFilter)
	{
		FARFilter Filter;
		Filter.bRecursiveClasses = false;
		Filter.bRecursivePaths = false;
		Filter.ClassPaths.Add(ClassToFilter->GetClassPathName());
		Filter.PackagePaths.Add(FName(*PathToFilter));
	
		return Filter;
	}
}
