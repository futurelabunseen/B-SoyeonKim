// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "WKGC_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGC_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UWKGC_AttackHit();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<class UParticleSystem> ParticleSystem;
};
