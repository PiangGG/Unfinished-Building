// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"

#include "CommonAbilityCost.generated.h"

class UCommonGameplayAbility;
/**
 * Base class for costs that a CommonGameplayAbility has (e.g., ammo or charges)
 * CommonGameplayAbility拥有的成本（例如弹药或次数）的基类。
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class COMMON_API UCommonAbilityCost : public UObject
{
	GENERATED_BODY()
public:
	UCommonAbilityCost()
	{
	}

	/**
	 * Checks if we can afford this cost.
	 *
	 * A failure reason tag can be added to OptionalRelevantTags (if non-null), which can be queried
	 * elsewhere to determine how to provide user feedback (e.g., a clicking noise if a weapon is out of ammo)
	 * 
	 * Ability and ActorInfo are guaranteed to be non-null on entry, but OptionalRelevantTags can be nullptr.
	 * 
	 * @return true if we can pay for the ability, false otherwise.
	 */
	/**
	*	检查我们是否负担得起这个成本。
	*	如果失败，可以将失败原因标签添加到OptionalRelevantTags（如果非空），可以在其他地方查询以确定如何提供用户反馈（例如，如果武器没有弹药，则发出点击声音）。
	*	在进入时，Ability和ActorInfo保证非空，但OptionalRelevantTags可以为nullptr。
	*	@return 如果我们能支付能力，则返回true，否则返回false。
	*/
	virtual bool CheckCost(const UCommonGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	/**
	 * Applies the ability's cost to the target
	 *
	 * Notes:
	 * - Your implementation don't need to check ShouldOnlyApplyCostOnHit(), the caller does that for you.
	  * - Ability and ActorInfo are guaranteed to be non-null on entry.
	 */
	
	/**
	* 将能力的成本应用于目标
	* 注意：
	* 你的实现不需要检查ShouldOnlyApplyCostOnHit()，调用者会为你检查。
	* 在进入时，Ability和ActorInfo保证非空。
	*/
	virtual void ApplyCost(const UCommonGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}

	/** If true, this cost should only be applied if this ability hits successfully */
	/** 如果为真，则应仅在此能力成功命中时应用此成本。 */
	bool ShouldOnlyApplyCostOnHit() const { return bOnlyApplyCostOnHit; }

protected:
	/** If true, this cost should only be applied if this ability hits successfully */
	/** 如果为真，则应仅在此能力成功命中时应用此成本。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	bool bOnlyApplyCostOnHit = false;
};
