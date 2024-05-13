// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA/WKGameplayAbility.h"
#include "WKGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGA_AttackHitCheck : public UWKGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWKGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackBuffEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDotDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> StunEffect;

	float CurrentLevel;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class AWKTA_Trace> TargetActorClass;
};
	