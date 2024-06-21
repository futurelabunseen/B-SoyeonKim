// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Attack.h"
#include "Character/WKCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameFramework/PlayerController.h"
#include "Tag/WKGameplayTag.h"
#include "GameplayTask.h"
#include "EngineUtils.h"

UWKGA_Attack::UWKGA_Attack()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	AbilityTags.AddTag(WKTAG_CHARACTER_ACTION_ATTACK);
}

void UWKGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		OnCancelledCallback();
	}

	AWKCharacterBase* BaseCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
	BaseCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FGameplayEventData EventData;

	if (IsFirstAttack)
		PlayMontage(EventData);
	else
	{
		UAbilityTask_WaitGameplayEvent* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			WKTAG_CHARACTER_ACTION_NEXTATTACK, nullptr, true, true);
		WaitEvent->EventReceived.AddDynamic(this, &ThisClass::PlayMontage);
		WaitEvent->ReadyForActivation();
	}
}

void UWKGA_Attack::PlayMontage(FGameplayEventData Data)
{
	UE_LOG(LogTemp, Log, TEXT("PlayAttack : PlayMontage"));
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("None"), ComboAttackMontage);
	PlayAttackTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	PlayAttackTask->OnBlendOut.AddDynamic(this, &ThisClass::OnCancelledCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayAttackTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelledCallback);
	PlayAttackTask->ReadyForActivation();
}

void UWKGA_Attack::OnCompleteCallback()
{
	UE_LOG(LogTemp, Log, TEXT("PlayAttack : OnCompleteCallback"));
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Attack::OnInterruptedCallback()
{
	UE_LOG(LogTemp, Log, TEXT("PlayAttack : OnInterruptedCallback"));
	bool bReplicatedEndAbility = true;


	//취소된 것이기 때문에 Camcelled true 설정
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Attack::OnCancelledCallback()
{
	UE_LOG(LogTemp, Log, TEXT("PlayAttack : OnCancelledCallback"));
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UWKGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UAbilitySystemComponent* AvaterActorASC = GetAbilitySystemComponentFromActorInfo();

	if (!NextAttackTag.MatchesTag(WKTAG_CHARACTER_ACTION_ATTACK_LASTCOMBO))
	{
		if (AvaterActorASC)
		{
			if (AvaterActorASC->HasMatchingGameplayTag(NextAttackTag))
			{
				return;
			}
		}
	}

	if (!AvaterActorASC->HasMatchingGameplayTag(WKTAG_CHARACTER_STATE_DEBUFF_STUN) && 
		!AvaterActorASC->HasMatchingGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD))
	{
		// TODO : Dead 상태 체크 필요
		AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
		WKCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}	
}
	