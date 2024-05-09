// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/WKGameplayAbility.h"
#include "WKGA_Skill_FlamingSword.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGA_Skill_FlamingSword : public UWKGameplayAbility
{
	GENERATED_BODY()

	public:
		UWKGA_Skill_FlamingSword();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION()
	void OnCompleteAbility();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ActiveSkillActionMontage;

	UPROPERTY()
	FTimerHandle AbilityEndTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AbilityEndTimeRate = 6.0f;
};
