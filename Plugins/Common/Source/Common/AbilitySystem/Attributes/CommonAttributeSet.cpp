// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAttributeSet.h"

#include "Common/AbilitySystem/CommonAbilitySystemComponent.h"

UCommonAttributeSet::UCommonAttributeSet()
{
}

UWorld* UCommonAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UCommonAbilitySystemComponent* UCommonAttributeSet::GetCommonAbilitySystemComponent() const
{
	return Cast<UCommonAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
