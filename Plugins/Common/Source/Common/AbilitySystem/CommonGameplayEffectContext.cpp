#include "CommonGameplayEffectContext.h"

#include "CommonAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

FCommonGameplayEffectContext* FCommonGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FCommonGameplayEffectContext::StaticStruct()))
	{
		return (FCommonGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FCommonGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer |转发到FGameplayEffectContextNetSerializer
	// Note: If FLyraGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	//注意：FCommonGameplayEffectContext:：NetSerialize（），则必须实现自定义NetSerializer，因为当前回退将不再足够。
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(FCommonGameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif

void FCommonGameplayEffectContext::SetAbilitySource(const ICommonAbilitySourceInterface* InObject, float InSourceLevel)
{
	//MakeWeakObjectPtr:推导初始值设定项类型的Helper函数
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const ICommonAbilitySourceInterface* FCommonGameplayEffectContext::GetAbilitySource() const
{
	return Cast<ICommonAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FCommonGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}