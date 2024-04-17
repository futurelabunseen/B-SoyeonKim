// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/WKAT_Trace.h"
#include "GA/TA/WKTA_Trace.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/WKCharacterBase.h"
#include "Warking.h"
#include "Attribute/WKCharacterAttributeSet.h"

UWKGA_AttackHitCheck::UWKGA_AttackHitCheck()
{
	
}

void UWKGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Log, TEXT("ActivateAbilityAttack========="));
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UWKAT_Trace* AttackTraceTask = UWKAT_Trace::CreateTask(this, AWKTA_Trace::StaticClass());
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

		// GetSet은 const로 지정되어 있어서 값을 변경하기 위해 의도적으로 const_cast를 사용. 좋은방법이 아님 GameplayEffect로 변경할 예정
		UWKCharacterAttributeSet* TargetAttribute = const_cast<UWKCharacterAttributeSet*>(TargetASC->GetSet<UWKCharacterAttributeSet>());

		if (!SourceAttribute || !SourceAttribute)
		{
			UE_LOG(LogTemp, Log, TEXT("UWKGA_AttackHitCheck::OnTraceResultCallback : Attribute not Found!"));
			return;
		}
		
		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
