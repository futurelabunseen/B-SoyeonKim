// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/WKGameAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"


UWKGameAttributeSet::UWKGameAttributeSet() :
	ControlPlayerNumRed(0.0f),
	ControlPlayerNumBlue(0.0f),
	ControlScoreRed(0.0f),
	ControlScoreBlue(0.0f),
	MaxControlScore(100.0f),
	ControlGaugeRed(0.0f),
	ControlGaugeBlue(0.0f),
	MaxControlGauge(1.0f)
{


}

void UWKGameAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, ControlPlayerNumRed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, ControlPlayerNumBlue, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, ControlScoreRed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, ControlScoreBlue, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, MaxControlScore, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, ControlGaugeRed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, ControlGaugeBlue, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWKGameAttributeSet, MaxControlGauge, COND_None, REPNOTIFY_Always);
}

void UWKGameAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{


}

bool UWKGameAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UWKGameAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	//TODO : 거점 점령 점수 달성한 팀 Win 처리
}

void UWKGameAttributeSet::OnRep_ControlPlayerNumRed(const FGameplayAttributeData& OldControlPlayerNumRed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, ControlPlayerNumRed, OldControlPlayerNumRed);
}

void UWKGameAttributeSet::OnRep_ControlPlayerNumBlue(const FGameplayAttributeData& OldControlPlayerNumBlue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, ControlPlayerNumBlue, OldControlPlayerNumBlue);
}

void UWKGameAttributeSet::OnRep_ControlScoreRed(const FGameplayAttributeData& OldControlScoreRed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, ControlScoreRed, OldControlScoreRed);
}

void UWKGameAttributeSet::OnRep_ControlScoreBlue(const FGameplayAttributeData& OldControlScoreBlue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, ControlScoreBlue, OldControlScoreBlue);
}

void UWKGameAttributeSet::OnRep_MaxControlScore(const FGameplayAttributeData& OldMaxControlScore)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, MaxControlScore, OldMaxControlScore);
}

void UWKGameAttributeSet::OnRep_ControlGaugeRed(const FGameplayAttributeData& OldControlGaugeRed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, ControlGaugeRed, OldControlGaugeRed);
}

void UWKGameAttributeSet::OnRep_ControlGaugeBlue(const FGameplayAttributeData& OldControlGaugeBlue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, ControlGaugeBlue, OldControlGaugeBlue);
}

void UWKGameAttributeSet::OnRep_MaxControlGauge(const FGameplayAttributeData& OldMaxControlGauge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWKGameAttributeSet, MaxControlGauge, OldMaxControlGauge);
}
