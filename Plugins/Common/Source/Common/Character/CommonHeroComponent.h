// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/PawnComponent.h"
#include "CommonHeroComponent.generated.h"

struct FGameplayAbilitySpecHandle;
class UCommonCameraMode;
/**
 * 负责为由玩家控制的角色（或模拟玩家的机器人）设置输入和摄像机处理的组件。这取决于PawnExtensionComponent来协调初始化。
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class COMMON_API UCommonHeroComponent : public UPawnComponent
{
	GENERATED_BODY()

public:

	UCommonHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the hero component if one exists on the specified actor. */
	/** 如果指定的角色上存在英雄组件，则返回英雄组件。 */
	UFUNCTION(BlueprintPure, Category = "Common|Hero")
	static UCommonHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCommonHeroComponent>() : nullptr); }

	/** Overrides the camera from an active gameplay ability */
	void SetAbilityCameraMode(TSubclassOf<UCommonCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Clears the camera override if it is set */
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);
//
// 	/** Adds mode-specific input config */
// 	void AddAdditionalInputConfig(const ULyraInputConfig* InputConfig);
//
// 	/** Removes a mode-specific input config if it has been added */
// 	void RemoveAdditionalInputConfig(const ULyraInputConfig* InputConfig);
//
// 	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
// 	bool IsReadyToBindInputs() const;
// 	
// 	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
// 	static const FName NAME_BindInputsNow;
//
// 	/** The name of this component-implemented feature */
// 	static const FName NAME_ActorFeatureName;
//
// 	//~ Begin IGameFrameworkInitStateInterface interface
// 	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
// 	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
// 	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
// 	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
// 	virtual void CheckDefaultInitialization() override;
// 	//~ End IGameFrameworkInitStateInterface interface
//
// protected:
//
// 	virtual void OnRegister() override;
// 	virtual void BeginPlay() override;
// 	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
//
// 	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
//
// 	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
// 	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
//
// 	void Input_Move(const FInputActionValue& InputActionValue);
// 	void Input_LookMouse(const FInputActionValue& InputActionValue);
// 	void Input_LookStick(const FInputActionValue& InputActionValue);
// 	void Input_Crouch(const FInputActionValue& InputActionValue);
// 	void Input_AutoRun(const FInputActionValue& InputActionValue);
//
	TSubclassOf<UCommonCameraMode> DetermineCameraMode() const;
// 	
// 	void OnInputConfigActivated(const FLoadedMappableConfigPair& ConfigPair);
// 	void OnInputConfigDeactivated(const FLoadedMappableConfigPair& ConfigPair);
//
// protected:
//
// 	/**
// 	 * Input Configs that should be added to this player when initializing the input. These configs
// 	 * will NOT be registered with the settings because they are added at runtime. If you want the config
// 	 * pair to be in the settings, then add it via the GameFeatureAction_AddInputConfig
// 	 * 
// 	 * NOTE: You should only add to this if you do not have a game feature plugin accessible to you.
// 	 * If you do, then use the GameFeatureAction_AddInputConfig instead. 
// 	 */
// 	PRAGMA_DISABLE_DEPRECATION_WARNINGS
// 	UE_DEPRECATED(5.3, "DefaultInputConfigs have been deprecated, use DefaultInputMappings instead")
// 	TArray<FMappableConfigPair> DefaultInputConfigs;
// 	PRAGMA_ENABLE_DEPRECATION_WARNINGS
// 	
// 	UPROPERTY(EditAnywhere)
// 	TArray<FInputMappingContextAndPriority> DefaultInputMappings;
// 	
	/** Camera mode set by an ability. */
	UPROPERTY()
	TSubclassOf<UCommonCameraMode> AbilityCameraMode;
//
	/** Spec handle for the last ability to set a camera mode. */
	/** 最后一个设置相机模式的能力的规格句柄. */
	FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;
//
// 	/** True when player input bindings have been applied, will never be true for non - players */
// 	bool bReadyToBindInputs;
};
