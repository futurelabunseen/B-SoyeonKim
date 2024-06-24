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

	TargetCharacter = Cast<AWKCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if (TargetCharacter)
	{
		TargetCharacter->Sprint(true);

		if (TargetCharacter->GetIsMoving())
		{
			if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				return;
			}
		}
		else
		{
			EndSprintAbility(Handle, ActorInfo, ActivationInfo);
		}

		TargetASC = ActorInfo->AbilitySystemComponent.Get();
	}
}

void UWKGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{		
	EndSprintAbility(Handle, ActorInfo, ActivationInfo);
}

void UWKGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWKGA_Sprint::ServerSetStopSprint_Implementation()
{
	CancelCost();
	StopSrpint();
}

void UWKGA_Sprint::EndSprintAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ServerSetStopSprint();

	StopSrpint();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWKGA_Sprint::StopSrpint()
{
	TargetCharacter = Cast<AWKCharacterPlayer>(GetActorInfo().AvatarActor.Get());
	if (TargetCharacter)
	{
		//CancelCost();
		TargetCharacter->Sprint(false);
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
