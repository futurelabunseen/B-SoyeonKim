// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "WKTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AWKTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug;
};
