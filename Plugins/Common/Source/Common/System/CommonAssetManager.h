#pragma once


#include "Engine/AssetManager.h"
//#include "CommonAssetManagerStartupJob.h"
#include "Templates/SubclassOf.h"
#include "CommonAssetManager.generated.h"

class UCommonGameData;
/**
 * UCommonAssetManager
 *
 *	Game implementation of the asset manager that overrides functionality and stores game-specific types.
 *	It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic.
 *	This class is used by setting 'AssetManagerClassName' in DefaultEngine.ini.
 *
 *	资产管理器的游戏实现，覆盖功能并存储游戏特定类型。
 *	大多数游戏都希望覆盖AssetManager，因为它提供了一个良好的位置用于游戏特定的加载逻辑。
 *	通过在DefaultEngine.ini中设置'AssetManagerClassName'，可以使用此类。
 */
UCLASS(Config = Game)
class UCommonAssetManager: public UAssetManager
{
	GENERATED_BODY()
public:

	UCommonAssetManager();

	// Returns the AssetManager singleton object.
	static UCommonAssetManager& Get();

	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	// 返回由TSoftObjectPtr引用的资产。如果尚未加载该资产，这将同步加载它。
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	// 返回由TSoftClassPtr引用的子类。如果尚未加载该资产，这将同步加载它。
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);
//
// 	// Logs all assets currently loaded and tracked by the asset manager.
// 	static void DumpLoadedAssets();
//
	const UCommonGameData& GetGameData();
// 	const ULyraPawnData* GetDefaultPawnData() const;
//
protected:
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		// Does a blocking load if needed
		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}
//
//
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();
//
	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);
//
// 	//~UAssetManager interface
// 	virtual void StartInitialLoading() override;
// #if WITH_EDITOR
// 	virtual void PreBeginPIE(bool bStartSimulate) override;
// #endif
// 	//~End of UAssetManager interface
//
	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);
//
// protected:
//
// 	// Global game data asset to use.
	UPROPERTY(Config)
	TSoftObjectPtr<UCommonGameData> CommonGameDataPath;
//
	// Loaded version of the game data
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;
//
// 	// Pawn data used when spawning player pawns if there isn't one set on the player state.
// 	UPROPERTY(Config)
// 	TSoftObjectPtr<ULyraPawnData> DefaultPawnData;
//
// private:
// 	// Flushes the StartupJobs array. Processes all startup work.
// 	void DoAllStartupJobs();
//
// 	// Sets up the ability system
// 	void InitializeGameplayCueManager();
//
// 	// Called periodically during loads, could be used to feed the status to a loading screen
// 	void UpdateInitialGameContentLoadPercent(float GameContentPercent);
//
// 	// The list of tasks to execute on startup. Used to track startup progress.
// 	TArray<FLyraAssetManagerStartupJob> StartupJobs;
//
// private:
// 	
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;
//
	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
	
};
template<typename AssetType>
AssetType* UCommonAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UCommonAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}