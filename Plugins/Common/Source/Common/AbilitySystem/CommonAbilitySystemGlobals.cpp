// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAbilitySystemGlobals.h"
#include "CommonGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonAbilitySystemGlobals)

struct FGameplayEffectContext;

UCommonAbilitySystemGlobals::UCommonAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}
FGameplayEffectContext* UCommonAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCommonGameplayEffectContext();
}
