// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonPerformanceSettings.h"

UCommonPlatformSpecificRenderingSettings::UCommonPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UCommonPlatformSpecificRenderingSettings* UCommonPlatformSpecificRenderingSettings::Get()
{
	UCommonPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

UCommonPerformanceSettings::UCommonPerformanceSettings()
{
	PerPlatformSettings.Initialize(UCommonPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	// 允许默认为所有统计信息
	FCommonPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (ECommonDisplayablePerformanceStat PerfStat : TEnumRange<ECommonDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}

}
