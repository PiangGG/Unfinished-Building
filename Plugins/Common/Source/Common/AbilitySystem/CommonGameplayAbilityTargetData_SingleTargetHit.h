#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"

#include "CommonGameplayAbilityTargetData_SingleTargetHit.generated.h"
class FArchive;
struct FGameplayEffectContextHandle;

/** Game-specific additions to SingleTargetHit tracking */
/** 对SingleTargetHit跟踪的特定游戏添加 */
USTRUCT()
struct FCommonGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

	FCommonGameplayAbilityTargetData_SingleTargetHit()
	: CartridgeID(-1)
	{ }

	virtual void AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const override;

	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FCommonGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}
	
};

template<>
struct TStructOpsTypeTraits<FCommonGameplayAbilityTargetData_SingleTargetHit> : public TStructOpsTypeTraitsBase2<FCommonGameplayAbilityTargetData_SingleTargetHit>
{
	enum
	{
		// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
		//目前，这是FGameplayAbilityTargetDataHandle网络序列化工作所必需的
		WithNetSerializer = true	
	};
};