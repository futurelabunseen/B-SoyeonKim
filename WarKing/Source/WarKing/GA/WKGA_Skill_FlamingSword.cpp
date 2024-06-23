// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Skill_FlamingSword.h"
#include "Character/WKCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tag/WKGameplayTag.h"
#include "WarKing.h"

UWKGA_Skill_FlamingSword::UWKGA_Skill_FlamingSword()
{
}

void UWKGA_Skill_FlamingSword::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());

	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWKGA_Skill_FlamingSword::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UWKGA_Skill_FlamingSword::OnCompleteCallback);
	PlayMontageTask->ReadyForActivation();

	ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		ASC->AddLooseGameplayTag(ActivateActionTag);
		ASC->AddReplicatedLooseGameplayTag(ActivateActionTag);
	}

	GetWorld()->GetTimerManager().
		SetTimer(AbilityEndTimer, this, &UWKGA_Skill_FlamingSword::OnCompleteAbility, AbilityEndTimeRate);
}

bool UWKGA_Skill_FlamingSword::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

	if (TargetCharacter->GetIsFalling())
	{
		return false;
	}

	return true;
}

void UWKGA_Skill_FlamingSword::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ASC->HasMatchingGameplayTag(ActivateActionTag))
	{
		ASC->RemoveLooseGameplayTag(ActivateActionTag);
		ASC->RemoveReplicatedLooseGameplayTag(ActivateActionTag);
	}
}

void UWKGA_Skill_FlamingSword::OnCompleteCallback()
{
	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(GetActorInfo().AvatarActor.Get());
	if (TargetCharacter && !TargetCharacter->HasGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD) 
		&& !TargetCharacter->HasGameplayTag(WKTAG_CHARACTER_STATE_DEBUFF_STUN))
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}	

	if (ASC->HasMatchingGameplayTag(ActivateActionTag))
	{
		ASC->RemoveLooseGameplayTag(ActivateActionTag);
		ASC->RemoveReplicatedLooseGameplayTag(ActivateActionTag);
	}
}
void UWKGA_Skill_FlamingSword::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Skill_FlamingSword::OnCompleteAbility()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
