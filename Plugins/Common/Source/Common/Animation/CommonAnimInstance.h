// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonAnimInstance.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS(Config = Game)
class COMMON_API UCommonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UCommonAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);
};
