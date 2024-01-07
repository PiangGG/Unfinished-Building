// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonCombatSet.h"

#include "Net/UnrealNetwork.h"
UCommonCombatSet::UCommonCombatSet()
{
}

void UCommonCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCommonCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCommonCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UCommonCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCommonCombatSet, BaseDamage, OldValue);
}

void UCommonCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCommonCombatSet, BaseHeal, OldValue);
}
