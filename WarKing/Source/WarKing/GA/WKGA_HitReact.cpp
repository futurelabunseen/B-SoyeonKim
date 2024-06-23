// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_HitReact.h"
#include "Tag/WKGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UWKGA_HitReact::UWKGA_HitReact()
{
    
}

void UWKGA_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
 
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        OnCompleteCallback();
    }

	if (!ActiveHitReactMontage)
	{
		return;
	}
   
    FString HitDirect = RemoveParentTag(TriggerEventData->EventTag, WKTAG_CHARACTER_ACTION_HITREACT);

    UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
    (this, TEXT("HitReactMontage"), ActiveHitReactMontage, 1.0f, *HitDirect);

    PlayMontageTask->OnCompleted.AddDynamic(this, &UWKGA_HitReact::OnCompleteCallback);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &UWKGA_HitReact::OnInterruptedCallback);
    PlayMontageTask->ReadyForActivation();

    UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

    // 어빌리티 취소 코드
    FGameplayTagContainer AbilityTagsToCancel;
    AbilityTagsToCancel.AddTag(WKTAG_CHARACTER_ACTION_ATTACK);
    AbilityTagsToCancel.AddTag(WKTAG_CHARACTER_ACTION_SKILL_FLAMINGSWORD);
    AbilityTagsToCancel.AddTag(WKTAG_CHARACTER_STATE_ISJUMPING);

    FGameplayTagContainer AbilityTagsToIgnore;
    AbilityTagsToIgnore.AddTag(WKTAG_CHARACTER_ACTION_HITREACT);

    ASC->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
}

void UWKGA_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWKGA_HitReact::OnCompleteCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = false;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_HitReact::OnInterruptedCallback()
{
    bool bReplicatedEndAbility = true;
    bool bWasCancelled = true;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FString UWKGA_HitReact::RemoveParentTag(const FGameplayTag& FullTag, const FGameplayTag& ParentTag)
{
    FString FullTagString = FullTag.ToString();
    FString ParentTagString = ParentTag.ToString();

    if (FullTagString.StartsWith(ParentTagString))
    {
        return FullTagString.RightChop(ParentTagString.Len() + 1);
    }

    return FullTagString;
}


