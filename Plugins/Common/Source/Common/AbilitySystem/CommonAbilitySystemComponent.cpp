// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAbilitySystemComponent.h"

#include "CommonAbilityTagRelationshipMap.h"
#include "CommonGlobalAbilitySystem.h"
#include "Common/CommonGameplayTags.h"
#include "Common/CommonLogChannels.h"
#include "Common/Animation/CommonAnimInstance.h"
#include "Common/System/CommonAssetManager.h"
#include "Common/System/CommonGameData.h"

UCommonAbilitySystemComponent::UCommonAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	
	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UCommonAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UCommonGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UCommonGlobalAbilitySystem>(GetWorld()))
	{
		GlobalAbilitySystem->UnregisterASC(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UCommonAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Notify all abilities that a new pawn avatar has been set
		// 通知所有能力已经设置了新的典当化身
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			UCommonGameplayAbility* CommonAbilityCDO = CastChecked<UCommonGameplayAbility>(AbilitySpec.Ability);

			if (CommonAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilityInstance : Instances)
				{
					UCommonGameplayAbility* CommonAbilityInstance = Cast<UCommonGameplayAbility>(AbilityInstance);
					if (CommonAbilityInstance)
					{
						// Ability instances may be missing for replays
						// 重播可能缺少能力实例
						CommonAbilityInstance->OnPawnAvatarSet();
					}
				}
			}
			else
			{
				CommonAbilityCDO->OnPawnAvatarSet();
			}
		}

		// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		if (UCommonGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UCommonGlobalAbilitySystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}

		if (UCommonAnimInstance* CommonAnimInst = Cast<UCommonAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			CommonAnimInst->InitializeWithAbilitySystem(this);
		}

		TryActivateAbilitiesOnSpawn();
	}
}

void UCommonAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc,
                                                          bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}
	
		UCommonGameplayAbility* CommonAbilityCDO = CastChecked<UCommonGameplayAbility>(AbilitySpec.Ability);
	
		if (CommonAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			// Cancel all the spawned instances, not the CDO.
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			for (UGameplayAbility* AbilityInstance : Instances)
			{
				UCommonGameplayAbility* CommonAbilityInstance = CastChecked<UCommonGameplayAbility>(AbilityInstance);
	
				if (ShouldCancelFunc(CommonAbilityInstance, AbilitySpec.Handle))
				{
					if (CommonAbilityInstance->CanBeCanceled())
					{
						CommonAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), CommonAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
					}
					else
					{
						UE_LOG(LogCommonAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *CommonAbilityInstance->GetName());
					}
				}
			}
		}
		else
		{
			// Cancel the non-instanced ability CDO.
			if (ShouldCancelFunc(CommonAbilityCDO, AbilitySpec.Handle))
			{
				// Non-instanced abilities can always be canceled.
				check(CommonAbilityCDO->CanBeCanceled());
				CommonAbilityCDO->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo(), bReplicateCancelAbility);
			}
		}
	}
}

void UCommonAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this](const UCommonGameplayAbility* CommonAbility, FGameplayAbilitySpecHandle Handle)
	{
		const ECommonAbilityActivationPolicy ActivationPolicy = CommonAbility->GetActivationPolicy();
		return ((ActivationPolicy == ECommonAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == ECommonAbilityActivationPolicy::WhileInputActive));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UCommonAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UCommonAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UCommonAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(CommonGameplayTags::TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops

	//
	// Process all abilities that activate when the input is held.
	// 处理保持输入时激活的所有能力。
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UCommonGameplayAbility* CommonAbilityCDO = CastChecked<UCommonGameplayAbility>(AbilitySpec->Ability);

				if (CommonAbilityCDO->GetActivationPolicy() == ECommonAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	// 处理所有输入按下此框的能力。
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					// 能力是活动的，所以传递输入事件。
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UCommonGameplayAbility* CommonAbilityCDO = CastChecked<UCommonGameplayAbility>(AbilitySpec->Ability);

					if (CommonAbilityCDO->GetActivationPolicy() == ECommonAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// 试着激活按压和保持的所有能力。
	// We do it all at once so that held inputs don't activate the ability
	// 我们一次完成所有操作，这样保持的输入就不会激活能力
	// and then also send a input event to the ability because of the press.
	// 然后也因为按下而向该能力发送输入事件。
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	// 处理所有输入释放此框架的能力。
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UCommonAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

bool UCommonAbilitySystemComponent::IsActivationGroupBlocked(ECommonAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case ECommonAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.|独立能力从不被阻挡。
			bBlocked = false;
		break;

	case ECommonAbilityActivationGroup::Exclusive_Replaceable:
	case ECommonAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.|如果没有任何东西阻挡，独占能力可以激活。
		bBlocked = (ActivationGroupCounts[(uint8)ECommonAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

void UCommonAbilitySystemComponent::AddAbilityToActivationGroup(ECommonAbilityActivationGroup Group,
	UCommonGameplayAbility* CommonAbility)
{
	check(CommonAbility);
	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);

	ActivationGroupCounts[(uint8)Group]++;

	const bool bReplicateCancelAbility = false;

	switch (Group)
	{
	case ECommonAbilityActivationGroup::Independent:
		// Independent abilities do not cancel any other abilities.|独立能力不会抵消任何其他能力。
			break;

	case ECommonAbilityActivationGroup::Exclusive_Replaceable:
	case ECommonAbilityActivationGroup::Exclusive_Blocking:
		CancelActivationGroupAbilities(ECommonAbilityActivationGroup::Exclusive_Replaceable, CommonAbility, bReplicateCancelAbility);
		break;

	default:
		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)ECommonAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)ECommonAbilityActivationGroup::Exclusive_Blocking];
	if (!ensure(ExclusiveCount <= 1))
	{
		UE_LOG(LogCommonAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
	}
}

void UCommonAbilitySystemComponent::RemoveAbilityFromActivationGroup(ECommonAbilityActivationGroup Group,
	UCommonGameplayAbility* CommonAbility)
{
	check(CommonAbility);
	check(ActivationGroupCounts[(uint8)Group] > 0);

	ActivationGroupCounts[(uint8)Group]--;
}

void UCommonAbilitySystemComponent::CancelActivationGroupAbilities(ECommonAbilityActivationGroup Group,
	UCommonGameplayAbility* IgnoreCommonAbility, bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this, Group, IgnoreCommonAbility](const UCommonGameplayAbility* CommonAbility, FGameplayAbilitySpecHandle Handle)
	{
		return ((CommonAbility->GetActivationGroup() == Group) && (CommonAbility != IgnoreCommonAbility));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UCommonAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	const TSubclassOf<UGameplayEffect> DynamicTagGE = UCommonAssetManager::GetSubclass(UCommonGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogCommonAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to find DynamicTagGameplayEffect [%s]."), *UCommonGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagGE, 1.0f, MakeEffectContext());
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	if (!Spec)
	{
		UE_LOG(LogCommonAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to make outgoing spec for [%s]."), *GetNameSafe(DynamicTagGE));
		return;
	}

	Spec->DynamicGrantedTags.AddTag(Tag);

	ApplyGameplayEffectSpecToSelf(*Spec);
}

void UCommonAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	const TSubclassOf<UGameplayEffect> DynamicTagGE = UCommonAssetManager::GetSubclass(UCommonGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogCommonAbilitySystem, Warning, TEXT("RemoveDynamicTagGameplayEffect: Unable to find gameplay effect [%s]."), *UCommonGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(Tag));
	Query.EffectDefinition = DynamicTagGE;

	RemoveActiveEffects(Query);
}

void UCommonAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle,
	FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}

void UCommonAbilitySystemComponent::SetTagRelationshipMapping(UCommonAbilityTagRelationshipMap* NewMapping)
{
	TagRelationshipMapping = NewMapping;
}

void UCommonAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags,
	FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}

void UCommonAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const UCommonGameplayAbility* CommonAbilityCDO = CastChecked<UCommonGameplayAbility>(AbilitySpec.Ability);
		CommonAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
	}
}

void UCommonAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// 我们不支持UGameplayAbility：：bReplicateInputDirect。
	// Use replicated events instead so that the WaitInputPress ability task works.
	// 请改用复制的事件，以便WaitInputPress功能任务正常工作。
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		// 调用InputPressed事件。此处未复制此内容。如果有人在监听，他们可能会将InputPressed事件复制到服务器。
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UCommonAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// 我们不支持UGameplayAbility：：bReplicateInputDirect。
	// Use replicated events instead so that the WaitInputRelease ability task works.
	// 请改用复制的事件，以便WaitInputRelease功能任务正常工作。
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		// InputReleased。此处未复制此内容。如果有人在监听，他们可能会将InputPressed事件复制到服务器。
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UCommonAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	UCommonGameplayAbility* CommonAbility = CastChecked<UCommonGameplayAbility>(Ability);

	AddAbilityToActivationGroup(CommonAbility->GetActivationGroup(), CommonAbility);
}

void UCommonAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle,
	UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			ClientNotifyAbilityFailed(Ability, FailureReason);
			return;
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void UCommonAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
	bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	UCommonGameplayAbility* CommonAbility = CastChecked<UCommonGameplayAbility>(Ability);

	RemoveAbilityFromActivationGroup(CommonAbility->GetActivationGroup(), CommonAbility);
}

void UCommonAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags,
	UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags,
	bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if (TagRelationshipMapping)
	{
		// Use the mapping to expand the ability tags into block and cancel tag
		//使用映射将功能标记扩展为块和取消标记
		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
	}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags, bExecuteCancelTags, ModifiedCancelTags);

	//@TODO: Apply any special logic like blocking input or movement
}

void UCommonAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags,
	UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);

	//@TODO: Apply any special logic like blocking input or movement
}

void UCommonAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability,
	const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void UCommonAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability,
	const FGameplayTagContainer& FailureReason)
{
	UE_LOG(LogCommonAbilitySystem, Warning, TEXT("Ability %s failed to activate (tags: %s)"), *GetPathNameSafe(Ability), *FailureReason.ToString());

	if (const UCommonGameplayAbility* CommonAbility = Cast<const UCommonGameplayAbility>(Ability))
	{
		CommonAbility->OnAbilityFailedToActivate(FailureReason);
	}	
}
