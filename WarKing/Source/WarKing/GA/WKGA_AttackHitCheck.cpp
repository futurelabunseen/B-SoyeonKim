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
	
}

void UWKGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("ActivateAbilityAttack========="));
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	// TODO: Multiplay AT, TA는 공부하고 적용해야 할 것 같다. -> 현재 오류
	UWKAT_Trace* AttackTraceTask = UWKAT_Trace::CreateTask(this, AWKTA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UWKGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

	// 정상적으로 동작
	//if (!WKCharacter->HasAuthority())
	//{
	//	ServerRPCAttack(0.1f);
	//}
}

void UWKGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{		
	// TODO: Multiplay AT, TA는 공부하고 적용해야 할 것 같다. -> 현재 오류
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
