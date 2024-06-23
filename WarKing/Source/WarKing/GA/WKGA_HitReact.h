// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/WKGameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "WKGA_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGA_HitReact : public UWKGameplayAbility
{
	GENERATED_BODY()
public:
    UWKGA_HitReact();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

    UFUNCTION()
    void OnCompleteCallback();

    UFUNCTION()
    void OnInterruptedCallback();
    
    FString RemoveParentTag(const FGameplayTag& FullTag, const FGameplayTag& ParentTag);

protected:
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> ActiveHitReactMontage;
};
