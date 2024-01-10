// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonInputTypeEnum.h"
#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPtr.h"

#include "CommonMappableConfigPair.generated.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

struct FMappableConfigPair;
class UPlayerMappableInputConfig;

/** A container to organize loaded player mappable configs to their CommonUI input type */
/** 一个容器，用于将加载的玩家可映射配置组织到其CommonUI输入类型 */
//FLoadedMappableConfigPair已被弃用。请改用FInputMappingContextAndPriority
USTRUCT(BlueprintType)
struct UE_DEPRECATED(5.3, "FLoadedMappableConfigPair has been deprecated. Please use FInputMappingContextAndPriority instead.") FLoadedMappableConfigPair
{
	GENERATED_BODY()

	FLoadedMappableConfigPair() = default;
	FLoadedMappableConfigPair(const UPlayerMappableInputConfig* InConfig, ECommonInputType InType, const bool InIsActive)
		: Config(InConfig)
		, Type(InType)
		, bIsActive(InIsActive)
	{}

	/** The player mappable input config that should be applied to the Enhanced Input subsystem */
	/**应应用于增强型输入子系统的玩家可映射输入配置*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<const UPlayerMappableInputConfig> Config = nullptr;

	/** The type of device that this mapping config should be applied to */
	/**应应用此映射配置的设备类型*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/** If this config is currently active. A config is marked as active when it's owning GFA is active */
	/**如果此配置当前处于活动状态。当拥有GFA的配置处于活动状态时，该配置被标记为活动*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsActive = false;
};

/** A container to organize potentially unloaded player mappable configs to their CommonUI input type */
/**一个容器，用于将可能卸载的玩家可映射配置组织到其CommonUI输入类型*/
USTRUCT()
struct UE_DEPRECATED(5.3, "FMappableConfigPair has been deprecated. Please use FInputMappingContextAndPriority instead.") FMappableConfigPair
{
	GENERATED_BODY()
	
	FMappableConfigPair() = default;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	/**
	 * The type of config that this is. Useful for filtering out configs by the current input device
	 * for things like the settings screen, or if you only want to apply this config when a certain
	 * input type is being used.
	 */
	/**
	*这是配置的类型。对于通过当前输入设备过滤掉配置很有用
	*例如设置屏幕，或者如果您只想在
	*正在使用输入类型。
	*/
	UPROPERTY(EditAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/**
	 * Container of platform traits that must be set in order for this input to be activated.
	 * If the platform does not have one of the traits specified it can still be registered, but cannot
	 * be activated. 
	 */
	/**
	*必须设置平台特征的容器才能激活此输入。
	*如果平台没有指定的特征之一，它仍然可以注册，但不能
	*被激活。
	*/
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DependentPlatformTraits;

	/**
	 * If the current platform has any of these traits, then this config will not be actived.
	 */
	/**
	*如果当前平台具有这些特征中的任何一个，则此配置将不会被激活。
	*/
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer ExcludedPlatformTraits;

	/** If true, then this input config will be activated when it's associated Game Feature is activated.
	 * This is normally the desirable behavior
	 */
	/**
	 *如果为true，则此输入配置将在其关联的游戏功能被激活时被激活。
	 *这通常是理想的行为
	 */
	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;

	/** Returns true if this config pair can be activated based on the current platform traits and settings. */
	/** 如果可以根据当前平台特征和设置激活此配置对，则返回true. */
	bool CanBeActivated() const;
	
	/**
	 * Registers the given config mapping with the local settings
	 */
	/**
	*使用本地设置注册给定的配置映射
	*/
	static bool RegisterPair(const FMappableConfigPair& Pair);

	/**
	 * Unregisters the given config mapping with the local settings
	 */
	/**
	*使用本地设置注销给定的配置映射
	*/
	static void UnregisterPair(const FMappableConfigPair& Pair);
};

PRAGMA_ENABLE_DEPRECATION_WARNINGS