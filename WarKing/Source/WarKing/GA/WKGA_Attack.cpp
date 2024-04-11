// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Attack.h"
#include "Character/WKCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/WKComboActionData.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GA/AT/WKAT_PlayMontageAndWait.h"
#include "GameplayTask.h"
#include "EngineUtils.h"

UWKGA_Attack::UWKGA_Attack()
{
	// ���͸��� �ϳ��� �����Ƽ �ν��Ͻ��� ����� ó�� (Primary Instance)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
}

void UWKGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
	CurrentComboData = WKCharacter->GetComboActionData();

	//Ability�� ����Ǵ� ���� ������ None ����
	WKCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//UAbilityTask_PlayMontageAndWait�� ���� Instance ����
	//UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), WKCharacter->GetComboActionMontage(), 1.0f, GetNextSection());
	UWKAT_PlayMontageAndWait* PlayAttackTask = Cast<UWKAT_PlayMontageAndWait>(UWKAT_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), WKCharacter->GetComboActionMontage(), 1.0f, GetNextSection()));

	//����X ��Ƽĳ��Ʈ ��������Ʈ Binding
	PlayAttackTask->OnCompleted.AddDynamic(this, &UWKGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UWKGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

void UWKGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}

	//if (GetOwningActorFromActorInfo()->GetLocalRole() != ROLE_Authority)
	//{
	//	AWKCharacterBase* Character = Cast<AWKCharacterBase>(GetAvatarActorFromActorInfo());
	//	ServerRPCPlayAnimation(Character);
	//}
}

void UWKGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;

	// ���� ���õ� �����Ͱ� �ִ��� Ȯ��
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UWKGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UWKGA_Attack::CheckComboInput()
{
	//���� �ð��� ������ Ÿ�̸Ӱ� �ߵ��� �Ǹ� Ÿ�̸� �ڵ��� �ϴ� ��ȿȭ ���Ѿ� �� 
	ComboTimerHandle.Invalidate();

	if (HasNextComboInput)
	{
		// ������ ��Ÿ�� ������ ������̶�� ������ ����
		if (CurrentCombo >= CurrentComboData->MaxComboCount)
		{
			OnCompleteCallback();
		}
		else
		{
			// MontageJumpToSection �Լ� ��ü�� Ability Ŭ���� �ȿ� ĳ���͸� �ƹ�Ÿ�� ������ �ִ� ��� ���ϰ� ����ϵ��� �����ϰ� ����
			MontageJumpToSection(GetNextSection());
			HasNextComboInput = false;
			StartComboTimer();
		}		
	}
}

bool UWKGA_Attack::ServerRPCPlayAnimation_Validate(AWKCharacterBase* Character)
{
	return true;
}

void UWKGA_Attack::ServerRPCPlayAnimation_Implementation(AWKCharacterBase* Character)
{
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		APlayerController* GetPlayerController = Cast<APlayerController>(Character->GetLocalViewingPlayerController());
		
		if (PlayerController && GetPlayerController != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				//AB_LOG(LogABNetwork, Log, TEXT("ServerRPCAttack IsLocalController(): %s"), *PlayerController->GetName());
				AWKGASCharacterPlayer* OtherPlayer = Cast<AWKGASCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					ClientRPCPlayAnimation();
				}
			}
		}
	}
}

void UWKGA_Attack::ClientRPCPlayAnimation_Implementation()
{
	// Jump To Montage

	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(GetOwningActorFromActorInfo());
	UE_LOG(LogTemp, Log, TEXT("ClientRPCPlayAnimation_Implementation : %s"), *WKCharacter->GetName());

}

void UWKGA_Attack::OnCompleteCallback()
{
	//UE_LOG(LogTemp, Log, TEXT("OnCompleteCallback"));
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Attack::OnInterruptedCallback()
{
	//UE_LOG(LogTemp, Log, TEXT("OnInterruptedCallback"));
	bool bReplicatedEndAbility = true;

	//��ҵ� ���̱� ������ Camcelled true ����
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

	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());
	WKCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// �� �ʱ�ȭ
	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
}

FName UWKGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}
