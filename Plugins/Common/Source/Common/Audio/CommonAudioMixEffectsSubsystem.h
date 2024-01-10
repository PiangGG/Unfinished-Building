// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CommonAudioMixEffectsSubsystem.generated.h"

class FSubsystemCollectionBase;
class UObject;
class USoundControlBus;
class USoundControlBusMix;
class USoundEffectSubmixPreset;
class USoundSubmix;
class UWorld;

USTRUCT()
struct FCommonAudioSubmixEffectsChain
{
	GENERATED_BODY()

	// Submix on which to apply the Submix Effect Chain Override
	// 应用混音效果链覆盖的混音。
	UPROPERTY(Transient)
	TObjectPtr<USoundSubmix> Submix = nullptr;

	// Submix Effect Chain Override (Effects processed in Array index order)
	// 混音效果链覆盖（按数组索引顺序处理的效果）。
	UPROPERTY(Transient)
	TArray<TObjectPtr<USoundEffectSubmixPreset>> SubmixEffectChain;
};

/**
 * This subsystem is meant to automatically engage default and user control bus mixes
 * to retrieve previously saved user settings and apply them to the activated user mix.
 * Additionally, this subsystem will automatically apply HDR/LDR Audio Submix Effect Chain Overrides
 * based on the user's preference for HDR Audio. Submix Effect Chain Overrides are defined in the
 * Lyra Audio Settings.
 */

/**
 *	此子系统旨在自动使用默认和用户控制总线混音，
 *	以检索先前保存的用户设置并将它们应用于已激活的用户混音。
 *	此外，该子系统将根据用户对HDR音频的喜好自动应用HDR/LDR音频混音效果链覆盖。
 *	混音效果链覆盖在Common音频设置中定义。
 */
UCLASS()
class COMMON_API UCommonAudioMixEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/** Called once all UWorldSubsystems have been initialized */
	/** 在所有UWorldSubsystems都被初始化后调用一次。 */
	virtual void PostInitialize() override;

	/** Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors */
	/** 在世界准备好开始游戏，但在游戏模式过渡到正确状态并在所有演员上调用BeginPlay之前调用。 */
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/** Set whether the HDR Audio Submix Effect Chain Override settings are applied */
	/** 设置是否应用HDR音频混音效果链覆盖设置。 */
	void ApplyDynamicRangeEffectsChains(bool bHDRAudio);
	
protected:
	void OnLoadingScreenStatusChanged(bool bShowingLoadingScreen);
	void ApplyOrRemoveLoadingScreenMix(bool bWantsLoadingScreenMix);
	
	// Called when determining whether to create this Subsystem
	// 在确定是否创建此子系统时调用。
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	// Default Sound Control Bus Mix retrieved from the Common Audio Settings
	// 从Common音频设置中检索到的默认声音控制总线混音。
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> DefaultBaseMix = nullptr;

	// Loading Screen Sound Control Bus Mix retrieved from the Common Audio Settings
	// 从Common音频设置中检索到的加载屏幕声音控制总线混音。
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> LoadingScreenMix = nullptr;

	// User Sound Control Bus Mix retrieved from the Common Audio Settings
	// 从Common音频设置中检索到的用户声音控制总线混音
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> UserMix = nullptr;

	// Overall Sound Control Bus retrieved from the Common Audio Settings and linked to the UI and game settings in CommonSettingsLocal
	// 从Common音频设置中检索到的总体声音控制总线，并链接到CommonSettingsLocal中的UI和游戏设置。
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> OverallControlBus = nullptr;

	// Music Sound Control Bus retrieved from the Common Audio Settings and linked to the UI and game settings in CommonSettingsLocal
	// 从Common音频设置中检索到的音乐声音控制总线，并链接到CommonSettingsLocal中的UI和游戏设置。
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> MusicControlBus = nullptr;

	// SoundFX Sound Control Bus retrieved from the Common Audio Settings and linked to the UI and game settings in CommonSettingsLocal
	// 从Common音频设置中检索到的SoundFX声音控制总线，并链接到CommonSettingsLocal中的UI和游戏设置
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> SoundFXControlBus = nullptr;

	// Dialogue Sound Control Bus retrieved from the Common Audio Settings and linked to the UI and game settings in CommonSettingsLocal
	// 从Common音频设置中检索到的对话声音控制总线，并链接到CommonSettingsLocal中的UI和游戏设置。
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> DialogueControlBus = nullptr;

	// VoiceChat Sound Control Bus retrieved from the Common Audio Settings and linked to the UI and game settings in CommonSettingsLocal
	// 从Common音频设置中检索到的语音聊天声音控制总线，并链接到CommonSettingsLocal中的UI和游戏设置。
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> VoiceChatControlBus = nullptr;

	// Submix Effect Chain Overrides to apply when HDR Audio is turned on
	// 在打开HDR音频时应用的混音效果链覆盖。
	UPROPERTY(Transient)
	TArray<FCommonAudioSubmixEffectsChain> HDRSubmixEffectChain;

	// Submix Effect hain Overrides to apply when HDR Audio is turned off
	// 在关闭HDR音频时应用的混音效果链覆盖。
	UPROPERTY(Transient)
	TArray<FCommonAudioSubmixEffectsChain> LDRSubmixEffectChain;

	bool bAppliedLoadingScreenMix = false;
};
