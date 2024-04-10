// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/WKGA_Jump.h"
#include "GameFramework/Character.h"
#include "GA/AT/WKAT_JumpAndWaitForLanding.h"

UWKGA_Jump::UWKGA_Jump()
{
	// 이렇게 액터마다 하나씩 존재하도록 해야 (Instance) 상태 보존 가능
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UWKGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (!bResult)
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (Character && Character->CanJump());
}

void UWKGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//ability Task로 Jump하도록
	UWKAT_JumpAndWaitForLanding* JumpAndWaitingForLandingTask = UWKAT_JumpAndWaitForLanding::CreateTask(this);

	// Landed 콜백 Delegate binding
	JumpAndWaitingForLandingTask->OnComplete.AddDynamic(this, &UWKGA_Jump::OnLandedCallback);
	JumpAndWaitingForLandingTask->ReadyForActivation();
}

void UWKGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 상태가 변하니까 const 없이
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UWKGA_Jump::OnLandedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
