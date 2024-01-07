// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CommonAttributeSet.generated.h"


struct FGameplayEffectSpec;

/**
 * This macro defines a set of helper functions for accessing and initializing attributes.|此宏定义了一组用于访问和初始化属性的辅助函数。
 *
 * The following example of the macro:|以下宏示例：
 *		ATTRIBUTE_ACCESSORS(UCommonHealthSet, Health)
 * will create the following functions:|将创建以下功能：
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** 
 * Delegate used to broadcast attribute events, some of these parameters may be null on clients: |用于广播属性事件的委托，其中一些参数在客户端上可能为null：
 * @param EffectInstigator	The original instigating actor for this event | 效果煽动者该事件的最初煽动者
 * @param EffectCauser		The physical actor that caused the change|造成变化的物理因素
 * @param EffectSpec		The full effect spec for this change|EffectSpec|此更改的完整效果规范
 * @param EffectMagnitude	The raw magnitude, this is before clamping|有效幅值原始幅值，这是在夹紧之前
 * @param OldValue			The value of the attribute before it was changed |更改前的属性值
 * @param NewValue			The value after it was changed |更改后的值
*/
DECLARE_MULTICAST_DELEGATE_SixParams(FCommonAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

/**
 * ULyraAttributeSet
 *
 *	Base attribute set class for the project.
 */
UCLASS()
class COMMON_API UCommonAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
 
    UCommonAttributeSet();

    UWorld* GetWorld() const override;

    UCommonAbilitySystemComponent* GetCommonAbilitySystemComponent() const;
};