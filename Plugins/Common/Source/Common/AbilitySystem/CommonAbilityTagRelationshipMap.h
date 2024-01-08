// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CommonAbilityTagRelationshipMap.generated.h"

/** Struct that defines the relationship between different ability tags */
/** 定义不同能力标记之间关系的结构*/
USTRUCT()
struct FCommonAbilityTagRelationship
{
	GENERATED_BODY()

	/** The tag that this container relationship is about. Single tag, but abilities can have multiple of these */
	/** 此容器关系所涉及的标记。单个标签，但能力可以有多个 */
	UPROPERTY(EditAnywhere, Category = Ability, meta = (Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	/** The other ability tags that will be blocked by any ability using this tag */
	/** 任何使用此标签的能力都会阻止的其他能力标签 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	/** The other ability tags that will be canceled by any ability using this tag */
	/** 任何使用此标记的能力都将取消的其他能力标记 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	/** If an ability has the tag, this is implicitly added to the activation required tags of the ability */
	/** 如果某个能力具有标记，则该标记会隐式添加到该能力的激活所需标记中 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	/** If an ability has the tag, this is implicitly added to the activation blocked tags of the ability */
	/** 如果某个能力具有标记，则该标记会隐式添加到该能力的激活阻止标记中 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationBlockedTags;
};

/**
 * Mapping of how ability tags block or cancel other abilities
 */
/** 能力标签如何阻止或取消其他能力的映射 */
UCLASS()
class COMMON_API UCommonAbilityTagRelationshipMap : public UDataAsset
{
	GENERATED_BODY()
private:
	/** The list of relationships between different gameplay tags (which ones block or cancel others) */
	/** 不同游戏标签之间的关系列表（哪些标签阻止或取消其他标签） */
	UPROPERTY(EditAnywhere, Category = Ability, meta=(TitleProperty="AbilityTag"))
	TArray<FCommonAbilityTagRelationship> AbilityTagRelationships;

public:
	/** Given a set of ability tags, parse the tag relationship and fill out tags to block and cancel */
	/** 给定一组能力标签，解析标签关系并填写标签以阻止和取消 */
	void GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutTagsToBlock, FGameplayTagContainer* OutTagsToCancel) const;

	/** Given a set of ability tags, add additional required and blocking tags */
	/** 给定一组能力标签，添加额外的必需标签和阻止标签 */
	void GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutActivationRequired, FGameplayTagContainer* OutActivationBlocked) const;

	/** Returns true if the specified ability tags are canceled by the passed in action tag */
	/** 如果传入的操作标记取消了指定的能力标记，则返回true */
	bool IsAbilityCancelledByTag(const FGameplayTagContainer& AbilityTags, const FGameplayTag& ActionTag) const;
};
