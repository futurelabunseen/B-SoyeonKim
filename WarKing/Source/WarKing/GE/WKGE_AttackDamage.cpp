// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/WKGE_AttackDamage.h"
#include "Attribute/WKCharacterAttributeSet.h"

UWKGE_AttackDamage::UWKGE_AttackDamage()
{
	// Instant: 한 Frame에 바로 생성
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;

	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UWKCharacterAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UWKCharacterAttributeSet, Health)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat DamageAmount(-30.0f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);

	HealthModifier.ModifierMagnitude = ModMagnitude;

	Modifiers.Add(HealthModifier);
}
