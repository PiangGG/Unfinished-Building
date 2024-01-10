// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Engine/PlatformSettings.h"
#include "CommonPerformanceSettings.generated.h"

enum class ECommonDisplayablePerformanceStat : uint8;
struct FGameplayTagQuery;
class UObject;

// Describes one platform-specific device profile variant that the user can choose from in the UI
// 描述用户可以在UI中选择的一种特定于平台的设备配置文件变体
USTRUCT()
struct FCommonQualityDeviceProfileVariant
{
	GENERATED_BODY()

	// The display name for this device profile variant (visible in the options screen)
	// 此设备配置文件变体的显示名称（在选项屏幕中可见）
	UPROPERTY(EditAnywhere)
	FText DisplayName;

	// The suffix to append to the base device profile name for the current platform
	// 要附加到当前平台的基本设备配置文件名称的后缀
	UPROPERTY(EditAnywhere)
	FString DeviceProfileSuffix;

	// The minimum required refresh rate to enable this mode
	// 启用此模式所需的最低刷新率
	// (e.g., if this is set to 120 Hz and the device is connected
	// （例如，如果将其设置为120 Hz并且设备已连接
	// to a 60 Hz display, it won't be available)
	// 对于60赫兹的显示器，它将不可用）
	UPROPERTY(EditAnywhere)
	int32 MinRefreshRate = 0;
};

// Describes a set of performance stats that the user can enable in settings,
// 描述用户可以在设置中启用的一组性能统计信息，,
// predicated on passing a visibility query on platform traits
// 基于传递关于平台特性的可见性查询
USTRUCT()
struct FCommonPerformanceStatGroup
{
	GENERATED_BODY()

	// A query on platform traits to determine whether or not it will be possible
	// 对平台特征的查询，以确定是否可能
	// to show a set of stats
	// 显示一组统计数据
	UPROPERTY(EditAnywhere, meta=(Categories = "Input,Platform.Trait"))
	FGameplayTagQuery VisibilityQuery;

	// The set of stats to allow if the query passes
	//如果查询通过，则允许的一组统计信息
	UPROPERTY(EditAnywhere)
	TSet<ECommonDisplayablePerformanceStat> AllowedStats;
};

// How hare frame pacing and overall graphics settings controlled/exposed for the platform?
// 如何控制/显示平台的hare帧步调和整体图形设置？
UENUM()
enum class ECommonFramePacingMode : uint8
{
	// Manual frame rate limits, user is allowed to choose whether or not to lock to vsync
	// 手动帧速率限制，允许用户选择是否锁定vsync
	DesktopStyle,

	// Limits handled by choosing present intervals driven by device profiles
	// 通过选择由设备配置文件驱动的当前间隔处理的限制
	ConsoleStyle,

	// Limits handled by a user-facing choice of frame rate from among ones allowed by device profiles for the specific device
	// 由面向用户从特定设备的设备配置文件允许的帧速率中选择帧速率所处理的限制
	MobileStyle
};
/**
 * 
 */
UCLASS(config=Game, defaultconfig)
class COMMON_API UCommonPlatformSpecificRenderingSettings : public UPlatformSettings
{
	GENERATED_BODY()
	
public:
	UCommonPlatformSpecificRenderingSettings();

	// Helper method to get the performance settings object, directed via platform settings
	// 获取性能设置对象的Helper方法，通过平台设置定向
	static const UCommonPlatformSpecificRenderingSettings* Get();

public:
	// The default variant suffix to append, should typically be a member of
	// UserFacingDeviceProfileOptions unless there is only one for the current platform
	//
	// Note that this will usually be set from platform-specific ini files, not via the UI
	
	//要附加的默认变体后缀通常应该是的成员
	//UserFacingDeviceProfileOptions，除非当前平台只有一个选项
	//
	//请注意，这通常是从特定于平台的ini文件中设置的，而不是通过UI
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	FString DefaultDeviceProfileSuffix;

	// The list of device profile variations to allow users to choose from in settings
	//
	// These should be sorted from slowest to fastest by target frame rate:
	//   If the current display doesn't support a user chosen refresh rate, we'll try
	//   previous entries until we find one that works
	//
	// Note that this will usually be set from platform-specific ini files, not via the UI
	
	//允许用户在设置中进行选择的设备配置文件变体列表
	//
	//这些应该按照目标帧速率从最慢到最快进行排序：
	//如果当前显示器不支持用户选择的刷新率，我们将尝试
	//之前的条目，直到我们找到一个有效的条目
	//
	//请注意，这通常是从特定于平台的ini文件中设置的，而不是通过UI
	UPROPERTY(EditAnywhere, Config, Category=DeviceProfiles)
	TArray<FCommonQualityDeviceProfileVariant> UserFacingDeviceProfileOptions;

	// Does the platform support granular video quality settings?
	// 该平台是否支持细粒度视频质量设置？
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsGranularVideoQualitySettings = true;

	// Does the platform support running the automatic quality benchmark (typically this should only be true if bSupportsGranularVideoQualitySettings is also true)
	// 平台是否支持运行自动质量基准测试（通常只有当bSupportsGranularVideoQualitySettings也是true时，这才应该是true）
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	bool bSupportsAutomaticVideoQualityBenchmark = true;

	// How is frame pacing controlled
	// 帧起搏是如何控制的
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings)
	ECommonFramePacingMode FramePacingMode = ECommonFramePacingMode::DesktopStyle;

	// Potential frame rates to display for mobile
	// Note: This is further limited by Lyra.DeviceProfile.Mobile.MaxFrameRate from the
	// platform-specific device profile and what the platform frame pacer reports as supported
	
	//为移动设备显示的潜在帧速率
	//注：这是进一步限制由莱拉。设备配置文件。可移动的的MaxFrameRate
	//特定于平台的设备配置文件以及平台框架起搏器报告支持的内容
	UPROPERTY(EditAnywhere, Config, Category=VideoSettings, meta=(EditCondition="FramePacingMode==ECommonFramePacingMode::MobileStyle", ForceUnits=Hz))
	TArray<int32> MobileFrameRateLimits;
};

//////////////////////////////////////////////////////////////////////

/**
 * Project-specific performance profile settings.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Common Performance Settings"))
class UCommonPerformanceSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UCommonPerformanceSettings();

private:
	// This is a special helper to expose the per-platform settings so they can be edited in the project settings
	// It never needs to be directly accessed
	UPROPERTY(EditAnywhere, Category = "PlatformSpecific")
	FPerPlatformSettings PerPlatformSettings;

public:
	// The list of frame rates to allow users to choose from in the various
	// "frame rate limit" video settings on desktop platforms
	UPROPERTY(EditAnywhere, Config, Category=Performance, meta=(ForceUnits=Hz))
	TArray<int32> DesktopFrameRateLimits;

	// The list of performance stats that can be enabled in Options by the user
	UPROPERTY(EditAnywhere, Config, Category=Stats)
	TArray<FCommonPerformanceStatGroup> UserFacingPerformanceStats;
};
