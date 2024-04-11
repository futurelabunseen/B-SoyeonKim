// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WKGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWKGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack(float AttackStartTime);

protected:

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
	