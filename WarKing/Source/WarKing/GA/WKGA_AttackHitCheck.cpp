// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/WKAT_Trace.h"
#include "GA/TA/WKTA_Trace.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/WKCharacterBase.h"
#include "Warking.h"
#include "Attribute/WKCharacterAttributeSet.h"
#include "Tag/WKGameplayTag.h"

UWKGA_AttackHitCheck::UWKGA_AttackHitCheck()
{
	
}

void UWKGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("ActivateAbilityAttack========="));
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;

	UWKAT_Trace* AttackTraceTask = UWKAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UWKGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

}

void UWKGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{		
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
		
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		
		if (!SourceASC || !TargetASC)
		{
			UE_LOG(LogTemp, Log, TEXT("UWKGA_AttackHitCheck::OnTraceResultCallback : ASC not Found!"));
			return;
		}
		
		const UWKCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UWKCharacterAttributeSet>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);

		if (EffectSpecHandle.IsValid())
		{
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParams;
			CueParams.EffectContext = CueContextHandle;

			if (!TargetASC->HasMatchingGameplayTag(WKTAG_EVENT_CHARACTER_ACTION_BLOCKATTACK))
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
	
				
				if (SourceASC->HasMatchingGameplayTag(WKTAG_CHARACTER_STATE_ISFLAMING))
				{
					FGameplayEffectSpecHandle DotEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDotDamageEffect, CurrentLevel);
					FGameplayEffectContextHandle DotCueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(DotEffectSpecHandle);
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DotEffectSpecHandle, TargetDataHandle);
				}

				TargetASC->ExecuteGameplayCue(WKTAG_GC_CHARACTER_ATTACKHIT, CueParams);
				
				// TODO : Hit React 방향대로 발동하도록 변경, RPC 날려 동기화
				FGameplayTagContainer Container;
				Container.AddTag(WKTAG_CHARACTER_ACTION_HITREACT);	
				TargetASC->TryActivateAbilitiesByTag(Container);
			}
			else
			{
				// Block Attack GC 발동
				TargetASC->ExecuteGameplayCue(WKTAG_GC_CHARACTER_BLOCKATTACK, CueParams);
			}
		}
		
		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect, CurrentLevel);

		if (BuffEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		// Actors 정보가 있는지 확인
		
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);


		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(WKTAG_GC_CHARACTER_ATTACKHIT, CueParam);
		}
	}


	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}


