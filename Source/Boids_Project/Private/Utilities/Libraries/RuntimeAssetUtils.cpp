
#include "Utilities/Libraries/RuntimeAssetUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"


FARFilter FRuntimeAssetUtils::GetFilterByClassAndPath(const UClass* ClassToFilter, const FString& PathToFilter, 
	bool bRecursivePath)
{
	FARFilter Filter;
	Filter.bRecursiveClasses = false;
	Filter.bRecursivePaths = bRecursivePath;
	Filter.ClassPaths.Add(ClassToFilter->GetClassPathName());
	Filter.PackagePaths.Add(FName(*PathToFilter));
	
	return Filter;
}

TArray<FAssetData> FRuntimeAssetUtils::GetFilteredAssetDataByClassAndPath(const UClass* ClassToFilter, 
	const FString& PathToFilter, bool bRecursivePath)
{		
	IAssetRegistry& AssetRegistry  = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	TArray<FAssetData> FilteredAssetDatas;
	
	FARFilter Filter = GetFilterByClassAndPath(ClassToFilter, PathToFilter, bRecursivePath);
	AssetRegistry.GetAssets(Filter, FilteredAssetDatas);

	return FilteredAssetDatas;
}


