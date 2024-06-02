// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "WKGE_ControlScoreExcutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGE_ControlScoreExcutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	UPROPERTY(EditAnywhere)
	float AdditiveScore;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute PlayNumAttribute;

	UPROPERTY(EditAnywhere)
	FGameplayAttribute TargetAttribute;
};
