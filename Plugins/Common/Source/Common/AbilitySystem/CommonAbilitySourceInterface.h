// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "CommonAbilitySourceInterface.generated.h"

class UObject;
class UPhysicalMaterial;
struct FGameplayTagContainer;

// This class does not need to be modified.
UINTERFACE()
class UCommonAbilitySourceInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class ICommonAbilitySourceInterface
{
	GENERATED_IINTERFACE_BODY()

	/**
	 * Compute the multiplier for effect falloff with distance|计算效果衰减与距离的乘积
	 * 
	 * @param Distance			Distance from source to target for ability calculations (distance bullet traveled for a gun, etc...)|用于能力计算的源到目标的距离
	 * @param SourceTags		Aggregated Tags from the source|来自源的聚合标记
	 * @param TargetTags		Aggregated Tags currently on the target|目标上当前的聚合标记
	 * 
	 * @return Multiplier to apply to the base attribute value due to distance|由于距离而应用于基本属性值的乘数
	 */
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const;

	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const;
};
