// COpyright

#pragma once

#include "CoreMinimal.h"


struct BOIDS_PROJECT_API FBoidConstants
{
	static const TCHAR* SpeciesTagName;
	
	static const TCHAR* BoidMaterialColorParameter;
	
	struct BOIDS_PROJECT_API Paths
	{
		static const TCHAR* SpeciesPath;
		static const TCHAR* BoundsPath;
		static const TCHAR* MaterialsPath;
	};
	
	struct BOIDS_PROJECT_API AssetNames
	{
		static const TCHAR* SpeciesPrefix;
		static const TCHAR* BoundsName;
		static const TCHAR* MaterialName;
		static const TCHAR* MaterialInstanceName;
	};
};