	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGamePhaseAbility.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "CommonGamePhaseSubsystem.generated.h"

template <typename T> class TSubclassOf;

class UCommonGamePhaseAbility;
class UObject;
struct FFrame;
struct FGameplayAbilitySpecHandle;

DECLARE_DYNAMIC_DELEGATE_OneParam(FCommonGamePhaseDynamicDelegate, const UCommonGamePhaseAbility*, Phase);
DECLARE_DELEGATE_OneParam(FCommonGamePhaseDelegate, const UCommonGamePhaseAbility* Phase);

DECLARE_DYNAMIC_DELEGATE_OneParam(FCommonGamePhaseTagDynamicDelegate, const FGameplayTag&, PhaseTag);
DECLARE_DELEGATE_OneParam(FCommonGamePhaseTagDelegate, const FGameplayTag& PhaseTag);

// Match rule for message receivers
UENUM(BlueprintType)
enum class EPhaseTagMatchType : uint8
{
	// An exact match will only receive messages with exactly the same channel
	// (e.g., registering for "A.B" will match a broadcast of A.B but not A.B.C)
	ExactMatch,

	// A partial match will receive any messages rooted in the same channel
	// (e.g., registering for "A.B" will match a broadcast of A.B as well as A.B.C)
	PartialMatch
};


/** Subsystem for managing Common's game phases using gameplay tags in a nested manner, which allows parent and child 
 * phases to be active at the same time, but not sibling phases.
 * Example:  Game.Playing and Game.Playing.WarmUp can coexist, but Game.Playing and Game.ShowingScore cannot. 
 * When a new phase is started, any active phases that are not ancestors will be ended.
 * Example: if Game.Playing and Game.Playing.CaptureTheFlag are active when Game.Playing.PostGame is started, 
 *     Game.Playing will remain active, while Game.Playing.CaptureTheFlag will end.
 */

/**
 *以嵌套方式使用游戏标签管理Common游戏阶段的子系统，允许家长和孩子
 *阶段同时处于活动状态，但不是同级阶段。
 *示例:Game.Playing和Game.Playing.WarmUp可以共存，但Game.Playing and Game.ShowingScore不能。
 *当一个新阶段开始时，任何不是祖先的活动阶段都将结束。
 *示例：如果Game.Playing和Game.Playing.CaptureTheFlag在游戏时处于活动状态。Game.Playing.PostGame开始，
 *Game.Playing保持活动状态，而Game.Playing.CaptureTheFlag将结束。
 */
UCLASS()
class COMMON_API UCommonGamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UCommonGamePhaseSubsystem();

	virtual void PostInitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void StartPhase(TSubclassOf<UCommonGamePhaseAbility> PhaseAbility, FCommonGamePhaseDelegate PhaseEndedCallback = FCommonGamePhaseDelegate());

	//TODO Return a handle so folks can delete these.  They will just grow until the world resets.|返回一个句柄，以便用户可以删除这些句柄。它们只会成长，直到世界重置
	//TODO Should we just occasionally clean these observers up?  It's not as if everyone will properly unhook them even if there is a handle.|我们是否应该偶尔清理一下这些观察者？即使有把手，也不是每个人都会正确地解开它们。
	void WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FCommonGamePhaseTagDelegate& WhenPhaseActive);
	void WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FCommonGamePhaseTagDelegate& WhenPhaseEnd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure = false, meta = (AutoCreateRefTerm = "PhaseTag"))
	bool IsPhaseActive(const FGameplayTag& PhaseTag) const;

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName="Start Phase", AutoCreateRefTerm = "PhaseEnded"))
	void K2_StartPhase(TSubclassOf<UCommonGamePhaseAbility> Phase, const FCommonGamePhaseDynamicDelegate& PhaseEnded);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "When Phase Starts or Is Active", AutoCreateRefTerm = "WhenPhaseActive"))
	void K2_WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, FCommonGamePhaseTagDynamicDelegate WhenPhaseActive);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Game Phase", meta = (DisplayName = "When Phase Ends", AutoCreateRefTerm = "WhenPhaseEnd"))
	void K2_WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, FCommonGamePhaseTagDynamicDelegate WhenPhaseEnd);

	void OnBeginPhase(const UCommonGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);
	void OnEndPhase(const UCommonGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);

private:
	struct FCommonGamePhaseEntry
	{
	public:
		FGameplayTag PhaseTag;
		FCommonGamePhaseDelegate PhaseEndedCallback;
	};

	TMap<FGameplayAbilitySpecHandle, FCommonGamePhaseEntry> ActivePhaseMap;

	struct FPhaseObserver
	{
	public:
		bool IsMatch(const FGameplayTag& ComparePhaseTag) const;
	
		FGameplayTag PhaseTag;
		EPhaseTagMatchType MatchType = EPhaseTagMatchType::ExactMatch;
		FCommonGamePhaseTagDelegate PhaseCallback;
	};

	TArray<FPhaseObserver> PhaseStartObservers;
	TArray<FPhaseObserver> PhaseEndObservers;

	friend class UCommonGamePhaseAbility;	
};
