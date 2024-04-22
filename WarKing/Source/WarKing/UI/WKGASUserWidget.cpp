// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UWKGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UWKGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
