// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"
#include "CommonGameplayAbility.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;

class AActor;
class AController;
// class ALyraCharacter;
// class ALyraPlayerController;
class APlayerController;
class FText;
// class ILyraAbilitySourceInterface;
class UAnimMontage;
// class ULyraAbilityCost;
// class ULyraAbilitySystemComponent;
// class ULyraCameraMode;
// class ULyraHeroComponent;
class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEffectSpec;
struct FGameplayEventData;

/**
 * ECommonAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.|定义一种能力的激活方式。
 *	
 */
UENUM(BlueprintType)
enum class ECommonAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.|当输入被触发时，尝试激活该功能。
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.|当输入处于活动状态时，不断尝试激活该能力。
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.当分配了化身时，尝试激活该功能。
	OnSpawn
};

/**
 * ECommonAbilityActivationGroup
 *
 *	Defines how an ability activates in relation to other abilities.|定义一种能力相对于其他能力的激活方式。
 *	
 */
UENUM(BlueprintType)
enum class ECommonAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.|能力独立于所有其他能力运行。
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.|能力被取消，取而代之的是其他专属能力。
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.|能力阻止所有其他专属能力激活。
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
};

/** Failure reason that can be used to play an animation montage when a failure occurs *///|发生故障时可用于播放动画蒙太奇的故障原因
USTRUCT(BlueprintType)
struct FCommonAbilityMontageFailureMessage
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	// All the reasons why this ability has failed |这个能力失败的所有原因
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimMontage> FailureMontage = nullptr;
};

/**
 * UCommonGameplayAbility
 *
 *	The base gameplay ability class used by this project.
 */
UCLASS()
class COMMON_API UCommonGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
};
