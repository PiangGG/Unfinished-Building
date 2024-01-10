#include "CommonMappableConfigPair.h"

#include "CommonUISettings.h"
#include "ICommonUIModule.h"
#include "Common/Settings/CommonSettingsLocal.h"
#include "Common/System/CommonAssetManager.h"

bool FMappableConfigPair::CanBeActivated() const
{
	const FGameplayTagContainer& PlatformTraits = ICommonUIModule::GetSettings().GetPlatformTraits();

	// If the current platform does NOT have all the dependent traits, then don't activate it
	// 如果当前平台没有所有依赖特征，那么不要激活它
	if (!DependentPlatformTraits.IsEmpty() && !PlatformTraits.HasAll(DependentPlatformTraits))
	{
		return false;
	}

	// If the platform has any of the excluded traits, then we shouldn't activate this config.
	// 如果平台有任何被排除的特征，那么我们不应该激活此配置。
	if (!ExcludedPlatformTraits.IsEmpty() && PlatformTraits.HasAny(ExcludedPlatformTraits))
	{
		return false;
	}

	return true;
}

bool FMappableConfigPair::RegisterPair(const FMappableConfigPair& Pair)
{
	UCommonAssetManager& AssetManager = UCommonAssetManager::Get();

	if (UCommonSettingsLocal* Settings = UCommonSettingsLocal::Get())
	{
		// Register the pair with the settings, but do not activate it yet
		// 使用设置注册配对，但尚未激活
		if (const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(Pair.Config))
		{
			Settings->RegisterInputConfig(Pair.Type, LoadedConfig, false);
			return true;
		}	
	}
	
	return false;
}

void FMappableConfigPair::UnregisterPair(const FMappableConfigPair& Pair)
{
	UCommonAssetManager& AssetManager = UCommonAssetManager::Get();

	if (UCommonSettingsLocal* Settings = UCommonSettingsLocal::Get())
	{
		if (const UPlayerMappableInputConfig* LoadedConfig = AssetManager.GetAsset(Pair.Config))
		{
			Settings->UnregisterInputConfig(LoadedConfig);
		}
	}
}
