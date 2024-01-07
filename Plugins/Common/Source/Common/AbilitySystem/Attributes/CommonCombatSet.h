// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "CommonAttributeSet.h"
#include "CommonCombatSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class COMMON_API UCommonCombatSet : public UCommonAttributeSet
{
	GENERATED_BODY()

public:

	UCommonCombatSet();

	ATTRIBUTE_ACCESSORS(UCommonCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UCommonCombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Common|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Common|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
