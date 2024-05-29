// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/WKGameWIdgetController.h"

#include "Attribute/WKGameAttributeSet.h"

void UWKGameWIdgetController::BroadcastInitialValues()
{
	const UWKGameAttributeSet* WKGameAttributeSet = CastChecked<UWKGameAttributeSet>(AttributeSet);

	OnControlPlayerNumRedChanged.Broadcast(WKGameAttributeSet->GetControlPlayerNumRed());
	OnControlPlayerNumBlueChanged.Broadcast(WKGameAttributeSet->GetControlPlayerNumBlue());
	OnControlScoreRedChanged.Broadcast(WKGameAttributeSet->GetControlScoreRed());
	OnControlScoreBlueChanged.Broadcast(WKGameAttributeSet->GetControlScoreBlue());
	OnMaxControlScoreChanged.Broadcast(WKGameAttributeSet->GetMaxControlScore());
	OnControlGaugeRedChanged.Broadcast(WKGameAttributeSet->GetControlGaugeRed());
	OnControlGaugeBlueChanged.Broadcast(WKGameAttributeSet->GetControlGaugeBlue());
	OnMaxControlGaugeChanged.Broadcast(WKGameAttributeSet->GetMaxControlGauge());
}

void UWKGameWIdgetController::BindCallbacksToDependencies()
{
	const UWKGameAttributeSet* WKGameAttributeSet = CastChecked<UWKGameAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetControlPlayerNumRedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnControlPlayerNumRedChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetControlPlayerNumBlueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnControlPlayerNumBlueChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetControlScoreRedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnControlScoreRedChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetControlScoreBlueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnControlScoreBlueChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetMaxControlScoreAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxControlScoreChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetControlGaugeRedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnControlGaugeRedChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetControlGaugeBlueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnControlGaugeBlueChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKGameAttributeSet->GetMaxControlGaugeAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxControlGaugeChanged.Broadcast(Data.NewValue);
		}
	);
}
