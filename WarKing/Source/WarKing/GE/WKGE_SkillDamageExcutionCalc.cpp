// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/WKGE_SkillDamageExcutionCalc.h"
#include "AbilitySystemComponent.h"
#include "Attribute/WKCharacterSkillAttributeSet.h"
#include "Attribute/WKCharacterAttributeSet.h"

void UWKGE_SkillDamageExcutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceActor && TargetActor)
		{
			const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UWKCharacterSkillAttributeSet::GetSkillRangeAttribute());
			const float MaxDamage = SourceASC->GetNumericAttributeBase(UWKCharacterSkillAttributeSet::GetSkillAttackRateAttribute());
			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);
			const float InvDamageRatio = 1.0f - Distance / MaxDamageRange;
			float Damage = InvDamageRatio * MaxDamage;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UWKCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}
