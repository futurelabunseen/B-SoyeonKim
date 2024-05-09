// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Skill_FlamingSword.h"
#include "Character/WKCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "WarKing.h"

UWKGA_Skill_FlamingSword::UWKGA_Skill_FlamingSword()
{
}

void UWKGA_Skill_FlamingSword::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (!TargetCharacter)
	{
		return;
	}

	if (!ActiveSkillActionMontage)
	{
		return;
	}

	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWKGA_Skill_FlamingSword::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UWKGA_Skill_FlamingSword::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();

	GetWorld()->GetTimerManager().
		SetTimer(AbilityEndTimer, this, &UWKGA_Skill_FlamingSword::OnCompleteAbility, AbilityEndTimeRate);
}

void UWKGA_Skill_FlamingSword::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWKGA_Skill_FlamingSword::OnCompleteCallback()
{
	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(GetActorInfo().AvatarActor.Get());
	if (TargetCharacter)
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
