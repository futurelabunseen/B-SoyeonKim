// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKGASWidgetComponent.h"
#include "UI/WKGASUserWidget.h"

void UWKGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	InitGASWidget();
}

void UWKGASWidgetComponent::InitGASWidget()
{
	UWKGASUserWidget* GASUserWidget = Cast<UWKGASUserWidget>(GetWidget());

	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
