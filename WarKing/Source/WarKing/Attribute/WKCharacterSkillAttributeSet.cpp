// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/WKCharacterSkillAttributeSet.h"

UWKCharacterSkillAttributeSet::UWKCharacterSkillAttributeSet() :
	SkillRange(800.0f),
	MaxSkillRange(1200.0f),
	SkillAttackRate(100.0f),
	MaxSkillAttackRate(200.0f),
	SkillEnergy(100.0f),
	MaxSkillEnergy(100.0f)
{
}

void UWKCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetSkillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if (Attribute == GetSkillAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillAttackRate());
	}
}
