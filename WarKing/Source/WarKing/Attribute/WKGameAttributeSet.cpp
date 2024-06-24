// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/WKGameAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"
#include "Tag/WKGameplayTag.h"


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

	if (!ASC)
	{
		ASC = GetOwningAbilitySystemComponentChecked();
	}

	//TODO : 거점 점령 점수 달성한 팀 Win 처리
	if (Data.EvaluatedData.Attribute == GetControlScoreBlueAttribute())
	{
		float CurrBlueControlScore = GetControlScoreBlue();
		float CurrMaxControlScore = GetMaxControlScore();
		SetControlScoreBlue(FMath::Clamp(CurrBlueControlScore, 0.0f, CurrMaxControlScore));

		if (CurrBlueControlScore >= CurrMaxControlScore)
		{
			OnWinnerTeam.Broadcast();
		}
	}
	else if (Data.EvaluatedData.Attribute == GetControlScoreRedAttribute())
	{
		float CurrRedControlScore = GetControlScoreRed();
		float CurrMaxControlScore = GetMaxControlScore();
		SetControlScoreRed(FMath::Clamp(CurrRedControlScore, 0.0f, CurrMaxControlScore));

		if (CurrRedControlScore >= CurrMaxControlScore)
		{
			OnWinnerTeam.Broadcast();
		}
	}
	else if (Data.EvaluatedData.Attribute == GetControlGaugeBlueAttribute())
	{
		float GaugeBlueValue = GetControlGaugeBlue();
		float MaxGaugeValue = GetMaxControlGauge();
		// Blue Team 점령률 달성
		if (GaugeBlueValue >= MaxGaugeValue)
		{
			if (ASC->HasMatchingGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_REDTEAM))
			{
				ASC->RemoveLooseGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_REDTEAM);
			}

			if (!ASC->HasMatchingGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_BLUETEAM))
			{
				ASC->AddLooseGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_BLUETEAM);
			}
		}
		else if (GaugeBlueValue < UE_KINDA_SMALL_NUMBER)
		{
			ASC->AddLooseGameplayTag(WKTAG_GAME_CONTROL_BLUEGAUGEZERO);
		}
		else
		{
			if (ASC->HasMatchingGameplayTag(WKTAG_GAME_CONTROL_BLUEGAUGEZERO))
			{
				ASC->RemoveLooseGameplayTag(WKTAG_GAME_CONTROL_BLUEGAUGEZERO);
			}
		}
	
		SetControlGaugeBlue(FMath::Clamp(GaugeBlueValue, 0.0f, MaxGaugeValue));
	}
	else if (Data.EvaluatedData.Attribute == GetControlGaugeRedAttribute())
	{
		float GaugeRedValue = GetControlGaugeRed();
		float MaxGaugeValue = GetMaxControlGauge();

		// Red Team 점령률 달성
		if (GaugeRedValue >= MaxGaugeValue)
		{
			if (ASC->HasMatchingGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_BLUETEAM))
			{
				ASC->RemoveLooseGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_BLUETEAM);
			}
			
			if (!ASC->HasMatchingGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_REDTEAM))
			{
				ASC->AddLooseGameplayTag(WKTAG_GAME_CONTROL_DOMINATE_REDTEAM);
			}
		}
		else if (GaugeRedValue < UE_KINDA_SMALL_NUMBER)
		{
			ASC->AddLooseGameplayTag(WKTAG_GAME_CONTROL_REDGAUGEZERO);
		}
		else
		{
			if (ASC->HasMatchingGameplayTag(WKTAG_GAME_CONTROL_REDGAUGEZERO))
			{
				ASC->RemoveLooseGameplayTag(WKTAG_GAME_CONTROL_REDGAUGEZERO);
			}
		}
		SetControlGaugeRed(FMath::Clamp(GaugeRedValue, 0.0f, MaxGaugeValue));
	}
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
