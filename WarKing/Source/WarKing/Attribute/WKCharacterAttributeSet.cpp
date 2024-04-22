// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/WKCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/WKGameplayTag.h"

UWKCharacterAttributeSet::UWKCharacterAttributeSet() : 
	AttackRange(100.0f),
	AttackRadius(50.f),
	AttackRate(30.0f),
	MaxAttackRange(300.0f),
	MaxAttackRadius(150.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Damage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UWKCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	//}

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UWKCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(CHARACTER_STATE_INVISIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;
}

void UWKCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		// Á×À¸¸é ÇØ´ç Target¿¡´Â IsDead Tag°¡ ºÎÂø
		Data.Target.AddLooseGameplayTag(CHARACTER_STATE_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}

