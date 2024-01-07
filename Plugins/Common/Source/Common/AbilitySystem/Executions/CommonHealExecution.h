// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CommonHealExecution.generated.h"

/**
 * 
 */
UCLASS()
class COMMON_API UCommonHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UCommonHealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
