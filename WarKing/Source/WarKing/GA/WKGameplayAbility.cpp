// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Tag/WKGameplayTag.h"

UWKGameplayAbility::UWKGameplayAbility()
{	
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating
	ActivationBlockedTags.AddTag(WKTAG_CHARACTER_STATE_ISDEAD);
	ActivationBlockedTags.AddTag(WKTAG_CHARACTER_STATE_DEBUFF_STUN);
}

void UWKGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
