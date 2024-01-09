#pragma once

#include "Engine/DataAsset.h"

#include "CommonGameData.generated.h"

class UGameplayEffect;
class UObject;

/**
 * ULyraGameData
 *
 *	Non-mutable data asset that contains global game data.
 *	包含全局游戏数据的非可变数据资产。
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Common Game Data", ShortTooltip = "Data asset containing global game data(UCommonGameData)."))
class UCommonGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UCommonGameData();

	// Returns the loaded game data.
	// 返回已加载的游戏数据。
	static const UCommonGameData& Get();

public:

	// Gameplay effect used to apply damage.  Uses SetByCaller for the damage magnitude.
	// 用于造成伤害的游戏效果。使用SetByCaller来设置伤害幅度。
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Damage Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffect_SetByCaller;

	// Gameplay effect used to apply healing.  Uses SetByCaller for the healing magnitude.
	// 用于施加治疗效果的游戏效果。使用SetByCaller来设置治疗幅度。
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Heal Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> HealGameplayEffect_SetByCaller;

	// Gameplay effect used to add and remove dynamic tags.
	// 用于添加和移除动态标签的游戏效果。
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};

