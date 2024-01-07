#pragma once

#include "GameplayEffectTypes.h"
#include "CommonGameplayEffectContext.generated.h"

class AActor;
class FArchive;
class ICommonAbilitySourceInterface;
class UObject;
class UPhysicalMaterial;

USTRUCT()
struct FCommonGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FCommonGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FCommonGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	/** Returns the wrapped FCommonGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	//从句柄返回包装的FCommonGameplayEffectContext，如果它不存在或类型错误，则返回nullptr
	static COMMON_API FCommonGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	//设置用作能力源的对象
	void SetAbilitySource(const ICommonAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const ICommonAbilitySourceInterface* GetAbilitySource() const;

	//Duplicate(复制)
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FCommonGameplayEffectContext* NewContext = new FCommonGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			//对命中结果进行深度复制
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FCommonGameplayEffectContext::StaticStruct();
	}

	/** Overridden to serialize new fields */
	//重写以序列化新字段
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Returns the physical material from the hit result if there is one */
	//返回命中结果中的物理材质（如果有）
	const UPhysicalMaterial* GetPhysicalMaterial() const;

public:
	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	//ID，用于识别同一弹药筒中的多个子弹
	UPROPERTY()
	int32 CartridgeID = -1;

protected:
	/** Ability Source object (should implement ILyraAbilitySourceInterface). NOT replicated currently */
	//能力源对象（应实现ICommonAbilitySourceInterface）。当前未复制
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FCommonGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FCommonGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};