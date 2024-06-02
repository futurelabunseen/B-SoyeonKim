// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/WKGE_ControlScoreExcutionCalc.h"

#include "Attribute/WKGameAttributeSet.h"

void UWKGE_ControlScoreExcutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		const float PlayerCount = SourceASC->GetNumericAttributeBase(PlayNumAttribute);
		float OutScore = AdditiveScore * PlayerCount;

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(TargetAttribute, EGameplayModOp::Additive, OutScore));
	}
}
