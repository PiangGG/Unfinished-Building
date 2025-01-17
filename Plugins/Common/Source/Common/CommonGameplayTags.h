﻿#pragma once

#include "NativeGameplayTags.h"

namespace CommonGameplayTags
{
	COMMON_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);
	
	// Declare all of the custom native tags that Common will use|声明Common将使用的所有自定义本机标记
	COMMON_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	COMMON_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);
	//
	COMMON_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	COMMON_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	COMMON_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);
	//
	// // These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	// LYRAGAME_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	// LYRAGAME_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);
	//
	// LYRAGAME_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);

	//AttributeSet
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Common_Damage_Message);

	//CommonAbilityComponent
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

	//CommonGameplayAbility
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ABILITY_SIMPLE_FAILURE_MESSAGE);
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ABILITY_PLAY_MONTAGE_FAILURE_MESSAGE);

	//CommonSetting
	COMMON_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Platform_Trait_BinauralSettingControlledByOS);
}