// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonHeroComponent.h"

#include "GameplayAbilitySpecHandle.h"
#include "Common/Camera/CommonCameraMode.h"

UCommonHeroComponent::UCommonHeroComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UCommonHeroComponent::SetAbilityCameraMode(TSubclassOf<UCommonCameraMode> CameraMode,
	const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UCommonHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

TSubclassOf<UCommonCameraMode> UCommonHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	// if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	// {
	// 	if (const ULyraPawnData* PawnData = PawnExtComp->GetPawnData<ULyraPawnData>())
	// 	{
	// 		return PawnData->DefaultCameraMode;
	// 	}
	// }
	//
	return nullptr;
}
