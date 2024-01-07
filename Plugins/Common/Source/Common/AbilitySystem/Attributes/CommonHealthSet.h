// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "CommonAttributeSet.h"

#include "CommonHealthSet.generated.h"

struct FGameplayEffectModCallbackData;

/**
 * UCommonHealthSet
 *
 *	Class that defines attributes that are necessary for taking damage.
 *	Attribute examples include: health, shields, and resistances.
 *	定义承受伤害所必需的属性。
 *	属性示例包括：生命值、护盾和抗性。
 */
UCLASS(BlueprintType)
class COMMON_API UCommonHealthSet : public UCommonAttributeSet
{
	GENERATED_BODY()

public:
	UCommonHealthSet();

	ATTRIBUTE_ACCESSORS(UCommonHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UCommonHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UCommonHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UCommonHealthSet, Damage);

	// Delegate when health changes due to damage/healing, some information may be missing on the client|当健康状况因损伤/愈合而改变时，委托，客户端上可能缺少某些信息
	mutable FCommonAttributeEvent OnHealthChanged;

	// Delegate when max health changes |最大生命值状况更改时委派
	mutable FCommonAttributeEvent OnMaxHealthChanged;

	// Delegate to broadcast when the health attribute reaches zero |当运行状况属性为零时要广播的委托
	mutable FCommonAttributeEvent OnOutOfHealth;

protected:

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	/**
	*在修改属性值之前调用。AttributeSet可以在此处进行其他修改。返回true继续，返回false放弃修改。
	*请注意，这仅在“执行”期间调用。例如，对属性的“基本值”的修改。在应用GameplayEffect时不会调用它，例如5秒+10移动速度buff。
	*/
	//在修改属性值前调用
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	/**
	*在执行GameplayEffect以修改属性的基本值之前调用。无法进行更多更改。
	*请注意，这仅在“执行”期间调用。例如，对属性的“基本值”的修改。在应用GameplayEffect时不会调用它，例如5秒+10移动速度buff。
	*/
	//在通过GE修改属性之前调用
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:

	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	// 当前运行状况属性。运行状况将由最大运行状况属性限制。运行状况对修改器是隐藏的，所以只有执行才能修改它。
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Common|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	// 当前最大健康属性。最大生命值是一个属性，因为游戏效果可以修改它。
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Common|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Used to track when the health reaches 0.|用于跟踪运行状况何时达到0。
	bool bOutOfHealth;

	// Store the health before any changes |在进行任何更改之前存储运行状况
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below
	//	元属性（请在下面保留非“有状态”的属性
	//	AllowPrivateAccess 蓝图可以访问私有变量
	// -------------------------------------------------------------------

	// Incoming healing. This is mapped directly to +Health |即将到来的治疗。这直接映射到+健康
	UPROPERTY(BlueprintReadOnly, Category="Common|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health |传入损坏。这直接映射到-健康
	UPROPERTY(BlueprintReadOnly, Category="Common|Health", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
