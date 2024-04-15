// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_PlayAttackMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/WKCharacterBase.h"
#include "Data/WKComboActionData.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "GameplayTask.h"
#include "EngineUtils.h"


UWKGA_PlayAttackMontage::UWKGA_PlayAttackMontage()
{

}

void UWKGA_PlayAttackMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Log, TEXT("ActiveTag PlayMontage================"));

	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
	CurrentComboData = WKCharacter->GetComboActionData();

	//Ability가 수행되는 동안 움직임 None 설정
	WKCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//UAbilityTask_PlayMontageAndWait에 대한 Instance 생성
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		TEXT("PlayAttack"), WKCharacter->GetComboActionMontage());

	//인자X 멀티캐스트 델리게이트 Binding
	//PlayAttackTask->OnCompleted.AddDynamic(this, &UWKGA_Attack::OnCompleteCallback);
	//PlayAttackTask->OnInterrupted.AddDynamic(this, &UWKGA_Attack::OnInterruptedCallback);
	//PlayAttackTask->OnCancelled.AddDynamic(this, &UWKGA_Attack::OnCancelledCallback);
	PlayAttackTask->ReadyForActivation();


	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}


void UWKGA_PlayAttackMontage::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UWKGA_PlayAttackMontage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
