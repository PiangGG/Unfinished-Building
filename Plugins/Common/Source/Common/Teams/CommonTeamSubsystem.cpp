// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonTeamSubsystem.h"

UCommonTeamSubsystem::UCommonTeamSubsystem()
{
}

bool UCommonTeamSubsystem::CanCauseDamage(const UObject* Instigator, const UObject* Target,
	bool bAllowDamageToSelf) const
{
	// if (bAllowDamageToSelf)
	// {
	// 	if ((Instigator == Target) || (FindPlayerStateFromActor(Cast<AActor>(Instigator)) == FindPlayerStateFromActor(Cast<AActor>(Target))))
	// 	{
	// 		return true;
	// 	}
	// }
	//
	// int32 InstigatorTeamId;
	// int32 TargetTeamId;
	// const ELyraTeamComparison Relationship = CompareTeams(Instigator, Target, /*out*/ InstigatorTeamId, /*out*/ TargetTeamId);
	// if (Relationship == ELyraTeamComparison::DifferentTeams)
	// {
	// 	return true;
	// }
	// else if ((Relationship == ELyraTeamComparison::InvalidArgument) && (InstigatorTeamId != INDEX_NONE))
	// {
	// 	// Allow damaging non-team actors for now, as long as they have an ability system component
	// 	//@TODO: This is temporary until the target practice dummy has a team assignment
	// 	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Cast<const AActor>(Target)) != nullptr;
	// }

	return false;
}
