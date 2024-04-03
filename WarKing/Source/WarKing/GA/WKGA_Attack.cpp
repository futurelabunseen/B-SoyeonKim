// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Attack.h"
#include "Character/WKCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"


UWKGA_Attack::UWKGA_Attack()
{
	// ���͸��� �ϳ��� �����Ƽ �ν��Ͻ��� ����� ó�� (Primary Instance)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UWKGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
	CurrentComboData = WKCharacter->GetComboActionData();

	//Ability�� ����Ǵ� ���� ������ None ����
	WKCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//UAbilityTask_PlayMontageAndWait�� ���� Instance ����
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), WKCharacter->GetComboActionMontage());
	//UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), WKCharacter->GetComboActionMontage(), 1.0f, GetNextSection());

	//����X ��Ƽĳ��Ʈ ��������Ʈ Binding
	PlayAttackTask->OnCompleted.AddDynamic(this, &UWKGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UWKGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();


}

void UWKGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UE_LOG(LogTemp, Log, TEXT("Begin"));
}

void UWKGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UWKGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
	WKCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UWKGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;

	//��ҵ� ���̱� ������ Camcelled true ����
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
