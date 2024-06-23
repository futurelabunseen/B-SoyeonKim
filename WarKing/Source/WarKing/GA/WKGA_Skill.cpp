// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Skill.h"
#include "Character/WKCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "WarKing.h"
#include "Tag/WKGameplayTag.h"

UWKGA_Skill::UWKGA_Skill()
{
}

void UWKGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());
	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWKGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UWKGA_Skill::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();

	// 어빌리티 취소 코드
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	FGameplayTagContainer AbilityTagsToCancel;
	AbilityTagsToCancel.AddTag(WKTAG_CHARACTER_ACTION_ATTACK);
	AbilityTagsToCancel.AddTag(WKTAG_CHARACTER_STATE_ISJUMPING);

	FGameplayTagContainer AbilityTagsToIgnore;

	ASC->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
}

bool UWKGA_Skill::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if (!TargetCharacter)
	{
		return false;
	}

	if (!ActiveSkillActionMontage)
	{
		return false;
	}

	if (TargetCharacter->HasGameplayTag(CooldownTag))
	{
		return false;
	}

	return true;
}

void UWKGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (TargetCharacter)
	{
		if (!TargetCharacter->HasGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD)
			&& !TargetCharacter->HasGameplayTag(WKTAG_CHARACTER_STATE_DEBUFF_STUN))
		{
			TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWKGA_Skill::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Skill::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
