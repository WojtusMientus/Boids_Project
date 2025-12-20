
#include "EditorUtilities/Libraries/EditorAssetUtils.h"
#include "AssetToolsModule.h"
#include "EditorAssetLibrary.h"
#include "FileHelpers.h"
#include "IAssetTools.h"


bool FEditorAssetUtils::EnsureDirectoryExist(const FString& DirectoryPath)
{
	if (!UEditorAssetLibrary::DoesDirectoryExist(DirectoryPath))
	{
		return UEditorAssetLibrary::MakeDirectory(DirectoryPath);
	}
	return true;
}

bool FEditorAssetUtils::SaveAsset(const UObject* AssetToSave)
{
	if (!AssetToSave)
	{
		return false;
	}
	
	AssetToSave->MarkPackageDirty();
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(AssetToSave->GetPackage());
	return UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true);
}

UObject* FEditorAssetUtils::CreateAssetInternal(const FString& AssetName, const FString& AssetPath,
                                                UClass* AssetClass, UFactory* Factory)
{
	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	return AssetTools.CreateAsset(AssetName, AssetPath, AssetClass, Factory);
}
