#include "CommonGameplayAbilityTargetData_SingleTargetHit.h"

#include "CommonGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonGameplayAbilityTargetData_SingleTargetHit)

struct FGameplayEffectContextHandle;

void FCommonGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context,
	bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);
	// Add game-specific data
	if (FCommonGameplayEffectContext* TypedContext = FCommonGameplayEffectContext::ExtractEffectContext(Context))
	{
		TypedContext->CartridgeID = CartridgeID;
	}
}

bool FCommonGameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}
