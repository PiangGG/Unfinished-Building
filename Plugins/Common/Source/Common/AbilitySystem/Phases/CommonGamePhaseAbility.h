// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/AbilitySystem/Abilitys/CommonGameplayAbility.h"
#include "CommonGamePhaseAbility.generated.h"

/**
 * UCommonGamePhaseAbility
 *
 * The base gameplay ability for any ability that is used to change the active game phase.
 * 用于更改活动游戏阶段的任何能力的基本游戏能力。
 */
UCLASS(Abstract, HideCategories = Input)
class COMMON_API UCommonGamePhaseAbility : public UCommonGameplayAbility
{
	GENERATED_BODY()

public:
	
	UCommonGamePhaseAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const FGameplayTag& GetGamePhaseTag() const { return GamePhaseTag; }

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	// Defines the game phase that this game phase ability is part of.  So for example,				|定义此游戏阶段能力所属的游戏阶段。例如，
	// if your game phase is GamePhase.RoundStart, then it will cancel all sibling phases.			|如果你的游戏阶段是GamePhase.RoundStart，则它将取消所有同级阶段。
	// So if you had a phase such as GamePhase.WaitingToStart that was active, starting				|所以，如果你有一个阶段，比如 GamePhase.WaitingToStart处于活动状态，正在启动
	// the ability part of RoundStart would end WaitingToStart.  However to get nested behaviors	|RoundStart的能力部分将结束WaitingToStart。但是，要获得嵌套行为
	// you can also nest the phases.  So for example, GamePhase.Playing.NormalPlay, is a sub-phase	|您也可以嵌套阶段。例如， GamePhase.Playing.NormalPlay,是一个子阶段
	// of the parent GamePhase.Playing, so changing the sub-phase to GamePhase.Playing.SuddenDeath,	|母游戏阶段的。GamePhase.Playing，所以将子阶段更改为GamePhase.Playing.SuddenDeath，
	// would stop any ability tied to GamePhase.Playing.*, but wouldn't end any ability				|将停止任何与GamePhase相关的能力。玩。*，但不会结束任何能力
	// tied to the GamePhase.Playing phase.															|绑定到 GamePhase.Playing phase.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Common|Game Phase")
	FGameplayTag GamePhaseTag;
};
