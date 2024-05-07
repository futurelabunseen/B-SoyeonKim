// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/WKOverlayWidgetController.h"

#include "Attribute/WKCharacterAttributeSet.h"

void UWKOverlayWidgetController::BroadcastInitialValues()
{
	const UWKCharacterAttributeSet* WKAttributeSet = CastChecked<UWKCharacterAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(WKAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(WKAttributeSet->GetMaxHealth());
	OnStaminaChanged.Broadcast(WKAttributeSet->GetHealth());
	OnMaxStaminaChanged.Broadcast(WKAttributeSet->GetMaxHealth());

}

void UWKOverlayWidgetController::BindCallbacksToDependencies()
{
	const UWKCharacterAttributeSet* WKAttributeSet = CastChecked<UWKCharacterAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKAttributeSet->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WKAttributeSet->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);
}
