// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Sprint.h"
#include "Character/WKCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "WarKing.h"
UWKGA_Sprint::UWKGA_Sprint()
{
}

void UWKGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if(TargetCharacter)
		TargetCharacter->Sprint(true);

	TargetASC = ActorInfo->AbilitySystemComponent.Get();
}

void UWKGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{		
	ServerSetStopSprint();
	StopSrpint();
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UWKGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWKGA_Sprint::ServerSetStopSprint_Implementation()
{
	StopSrpint();
}

void UWKGA_Sprint::StopSrpint()
{
	AWKCharacterPlayer* TargetCharacter = Cast<AWKCharacterPlayer>(GetActorInfo().AvatarActor.Get());
	if (TargetCharacter)
	{
		CancelCost();
		TargetCharacter->Sprint(false);
		//WKGAS_LOG(LogWKGAS, Log, TEXT("%s"), *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"),
		//	CurrentActorInfo->AvatarActor.Get()->GetOwner()->GetLocalRole())));
	}
}

void UWKGA_Sprint::CancelCost()
{  
	// Check if the ability system component is valid
	if (TargetASC && CostGameplayEffectClass)
	{
		TargetASC->RemoveActiveGameplayEffectBySourceEffect(CostGameplayEffectClass, TargetASC);
	}
}
