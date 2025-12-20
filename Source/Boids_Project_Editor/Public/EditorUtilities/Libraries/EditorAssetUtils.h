// Copyright WojtusMientus

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceConstant.h"


struct FEditorAssetUtils
{
	static bool EnsureDirectoryExist(const FString& DirectoryPath);
	
	static bool SaveAsset(const UObject* AssetToSave);
	
	template <typename T>
	static T* CreateAsset(const FString& AssetName, const FString& AssetPath, UFactory* Factory = nullptr)
	{
		return Cast<T>(CreateAssetInternal(AssetName, AssetPath, T::StaticClass(), Factory));
	}
	
private:
	
	static UObject* CreateAssetInternal(const FString& AssetName, const FString& AssetPath, UClass* AssetClass, 
		UFactory* Factory);
	
};
