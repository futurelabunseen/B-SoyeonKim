// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/WKCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Character/WKCharacterPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Enum/WKCharacterHitType.h"
#include "Tag/WKGameplayTag.h"

UWKCharacterAttributeSet::UWKCharacterAttributeSet() : 
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(300.0f),
	MaxStamina(100.0f),
	Damage(0.0f)
{
	InitHealth(GetMaxHealth());
	InitStamina(GetMaxStamina());
}

void UWKCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWKCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UWKCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
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
			if (Data.Target.HasMatchingGameplayTag(WKTAG_CHARACTER_STATE_INVISIBLE))
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

	AWKCharacterPlayer* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetCharacter = Cast<AWKCharacterPlayer>(Data.Target.AbilityActorInfo->AvatarActor.Get());
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{	
		// Handle stamina changes.
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));

		if (GetStamina() <= KINDA_SMALL_NUMBER)
		{
			if (TargetCharacter)
				TargetCharacter->ServerSetStopSprint();
		}
	}

	//if((GetHealth() <= 0.0f) && !bOutOfHealth)
	//{
	//	UE_LOG(LogTemp, Log, TEXT(" PostGameplayEffectExecute DEAD : %f"), GetDamage());
	//	// Á×À¸¸é ÇØ´ç Target¿¡´Â IsDead Tag°¡ ºÎÂø
	//	Data.Target.AddLooseGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD);
	//	OnOutOfHealth.Broadcast();
	//}

	//bOutOfHealth = (GetHealth() <= 0.0f);
}

void UWKCharacterAttributeSet::ResetAttributeSetData()
{
	SetHealth(GetMaxHealth());
	SetStamina(GetMaxStamina());
}

void UWKCharacterAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UWKCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKCharacterAttributeSet, Health, OldHealth);
}

void UWKCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UWKCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKCharacterAttributeSet, Stamina, OldStamina);
}

void UWKCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKCharacterAttributeSet, MaxStamina, OldMaxStamina);
}

