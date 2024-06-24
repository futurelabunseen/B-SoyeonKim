// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/WKGameplayAbility.h"
#include "WKGA_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGA_Sprint : public UWKGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWKGA_Sprint();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(Server, Reliable)
	void ServerSetStopSprint();
	void ServerSetStopSprint_Implementation();

	void EndSprintAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	void StopSrpint();
	void CancelCost();
	class AWKCharacterPlayer* TargetCharacter;
	UAbilitySystemComponent* TargetASC;
};
