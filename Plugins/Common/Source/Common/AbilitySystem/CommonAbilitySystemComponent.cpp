// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAbilitySystemComponent.h"

UCommonAbilitySystemComponent::UCommonAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// InputPressedSpecHandles.Reset();
	// InputReleasedSpecHandles.Reset();
	// InputHeldSpecHandles.Reset();
	//
	// FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UCommonAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc,
	bool bReplicateCancelAbility)
{
	// ABILITYLIST_SCOPE_LOCK();
	// for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	// {
	// 	if (!AbilitySpec.IsActive())
	// 	{
	// 		continue;
	// 	}
	//
	// 	ULyraGameplayAbility* LyraAbilityCDO = CastChecked<ULyraGameplayAbility>(AbilitySpec.Ability);
	//
	// 	if (LyraAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
	// 	{
	// 		// Cancel all the spawned instances, not the CDO.
	// 		TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
	// 		for (UGameplayAbility* AbilityInstance : Instances)
	// 		{
	// 			ULyraGameplayAbility* LyraAbilityInstance = CastChecked<ULyraGameplayAbility>(AbilityInstance);
	//
	// 			if (ShouldCancelFunc(LyraAbilityInstance, AbilitySpec.Handle))
	// 			{
	// 				if (LyraAbilityInstance->CanBeCanceled())
	// 				{
	// 					LyraAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), LyraAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
	// 				}
	// 				else
	// 				{
	// 					UE_LOG(LogLyraAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *LyraAbilityInstance->GetName());
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// Cancel the non-instanced ability CDO.
	// 		if (ShouldCancelFunc(LyraAbilityCDO, AbilitySpec.Handle))
	// 		{
	// 			// Non-instanced abilities can always be canceled.
	// 			check(LyraAbilityCDO->CanBeCanceled());
	// 			LyraAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo(), bReplicateCancelAbility);
	// 		}
	// 	}
	// }
}
