// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"
#include "CommonGameplayAbility.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;

class AActor;
class AController;
class ACommonCharacter;
class ACommonPC;
class APlayerController;
class FText;
 class ICommonAbilitySourceInterface;
class UAnimMontage;
class UCommonAbilityCost;
class UCommonAbilitySystemComponent;
class UCommonCameraMode;
class UCommonHeroComponent;
class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEffectSpec;
struct FGameplayEventData;

/**
 * ECommonAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.|定义一种能力的激活方式。
 *	
 */
UENUM(BlueprintType)
enum class ECommonAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.|当输入被触发时，尝试激活该功能。
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.|当输入处于活动状态时，不断尝试激活该能力。
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.当分配了化身时，尝试激活该功能。
	OnSpawn
};

/**
 * ECommonAbilityActivationGroup
 *
 *	Defines how an ability activates in relation to other abilities.|定义一种能力相对于其他能力的激活方式。
 *	
 */
UENUM(BlueprintType)
enum class ECommonAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.|能力独立于所有其他能力运行。
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.|能力被取消，取而代之的是其他专属能力。
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.|能力阻止所有其他专属能力激活。
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
};

/** Failure reason that can be used to play an animation montage when a failure occurs *///|发生故障时可用于播放动画蒙太奇的故障原因
USTRUCT(BlueprintType)
struct FCommonAbilityMontageFailureMessage
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	// All the reasons why this ability has failed |这个能力失败的所有原因
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimMontage> FailureMontage = nullptr;
};

/**
 * UCommonGameplayAbility
 *
 *	The base gameplay ability class used by this project.
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project(CommonGameplayAbility)."))
class COMMON_API UCommonGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	friend class UCommonAbilitySystemComponent;

public:

	UCommonGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	UCommonAbilitySystemComponent* GetCommonAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	ACommonPC* GetCommonPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	ACommonCharacter* GetCommonCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	UCommonHeroComponent* GetHeroComponentFromActorInfo() const;

	ECommonAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	ECommonAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	// Returns true if the requested activation group is a valid transition.
	// 如果请求的激活组是有效的过渡，则返回true.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Common|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(ECommonAbilityActivationGroup NewGroup) const;

	// Tries to change the activation group.  Returns true if it successfully changed.
	// 如果请求的激活组是有效的过渡，则返回true.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Common|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(ECommonAbilityActivationGroup NewGroup);

	// Sets the ability's camera mode.
	// 设置能力的相机模式。
	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	void SetCameraMode(TSubclassOf<UCommonCameraMode> CameraMode);

	// Clears the ability's camera mode.  Automatically called if needed when the ability ends.
	// 清除能力的相机模式。在需要时，当能力结束时会自动调用。
	UFUNCTION(BlueprintCallable, Category = "Common|Ability")
	void ClearCameraMode();

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
	{
		NativeOnAbilityFailedToActivate(FailedReason);
		ScriptOnAbilityFailedToActivate(FailedReason);
	}
protected:

	// Called when the ability fails to activate|当能力无法激活时调用
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	// Called when the ability fails to activate
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~End of UGameplayAbility interface

	virtual void OnPawnAvatarSet();

	virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const ICommonAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();

	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

protected:

	// Defines how this ability is meant to activate.|定义此功能的激活方式。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Common|Ability Activation")
	ECommonAbilityActivationPolicy ActivationPolicy;

	// Defines the relationship between this ability activating and other abilities activating.|定义此能力激活与其他能力激活之间的关系。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Common|Ability Activation")
	ECommonAbilityActivationGroup ActivationGroup;

	// Additional costs that must be paid to activate this ability|激活此功能必须支付的额外费用
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UCommonAbilityCost>> AdditionalCosts;

	// Map of failure tags to simple error messages |故障标记到简单错误消息的映射
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	TMap<FGameplayTag, FText> FailureTagToUserFacingMessages;

	// Map of failure tags to anim montages that should be played with them |失败标签到应该与之一起播放的动画蒙太奇的地图
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> FailureTagToAnimMontage;

	// If true, extra information should be logged when this ability is canceled. This is temporary, used for tracking a bug.
	// 如果为true，则应在取消此功能时记录额外信息。这是临时的，用于跟踪错误。
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	bool bLogCancelation;

	// Current camera mode set by the ability.|该功能设置的当前相机模式。
	TSubclassOf<UCommonCameraMode> ActiveCameraMode;
};
