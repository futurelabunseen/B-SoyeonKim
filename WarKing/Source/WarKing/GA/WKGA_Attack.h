// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA/WKGameplayAbility.h"
#include "WKGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGA_Attack : public UWKGameplayAbility
{
	GENERATED_BODY()

public:
	UWKGA_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION()
	void OnCancelledCallback();

	UFUNCTION()
	void PlayMontage(FGameplayEventData Data);

protected:
	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;


private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint8 IsFirstAttack : 1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FGameplayTag NextAttackTag;

	UPROPERTY()
	TObjectPtr<class UWKComboActionData> CurrentComboData;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> CancelTags;
};
