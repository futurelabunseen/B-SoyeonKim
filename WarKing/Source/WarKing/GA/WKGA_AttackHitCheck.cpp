// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/WKAT_Trace.h"
#include "GA/TA/WKTA_Trace.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/WKCharacterBase.h"
#include "Warking.h"

UWKGA_AttackHitCheck::UWKGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UWKGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TODO: Multiplay AT, TA는 공부하고 적용해야 할 것 같다. -> 현재 오류
	//UWKAT_Trace* AttackTraceTask = UWKAT_Trace::CreateTask(this, AWKTA_Trace::StaticClass());
	//AttackTraceTask->OnComplete.AddDynamic(this, &UWKGA_AttackHitCheck::OnTraceResultCallback);
	//AttackTraceTask->ReadyForActivation();
	AWKCharacterBase* WKCharacter = CastChecked<AWKCharacterBase>(ActorInfo->AvatarActor.Get());

	UE_LOG(LogTemp, Log, TEXT("ActivateAbilityAttack : %s"), *WKCharacter->GetName());

	// 정상적으로 동작
	//if (!WKCharacter->HasAuthority())
	//{
	//	ServerRPCAttack(0.1f);
	//}
}
void UWKGA_AttackHitCheck::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}
void UWKGA_AttackHitCheck::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
}

void UWKGA_AttackHitCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UWKGA_AttackHitCheck::OnCompleteCallback()
{
}
void UWKGA_AttackHitCheck::OnInterruptedCallback()
{
}
bool UWKGA_AttackHitCheck::ServerRPCAttack_Validate(float AttackStartTime)
{
	return true;
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_AttackHitCheck::ServerRPCAttack_Implementation(float AttackStartTime)
{

	UE_LOG(LogTemp, Log, TEXT("ServerRPCAttack_Implementation"));

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{		
	// TODO: Multiplay AT, TA는 공부하고 적용해야 할 것 같다. -> 현재 오류
	//if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	//{
	//	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
	//	UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
	//}

	//bool bReplicatedEndAbility = true;
	//bool bWasCancelled = false;
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
