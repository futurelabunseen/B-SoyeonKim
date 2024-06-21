// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/WKGE_RemoveTagCalculation.h"

#include "AbilitySystemComponent.h"
#include "Tag/WKGameplayTag.h"

void UWKGE_RemoveTagCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UE_LOG(LogTemp, Log, TEXT("UWKGE_RemoveTagCalculation"));

	if (!TargetASC) return;

	if (CancelTags.IsEmpty()) return;

	for (const FGameplayTag& CancelTag : CancelTags)
	{
		if (TargetASC->HasMatchingGameplayTag(CancelTag))
		{
			TargetASC->RemoveLooseGameplayTag(CancelTag);
		}
	}
}
