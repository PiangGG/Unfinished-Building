// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CommonDamageExecution.generated.h"


/**
 * UCommonDamageExecution
 *
 *	Execution used by gameplay effects to apply damage to the health attributes.
 *	游戏效果用来对生命值属性造成伤害的执行。
 */
UCLASS()
class COMMON_API UCommonDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UCommonDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
