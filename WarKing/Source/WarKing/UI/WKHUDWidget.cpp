// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKHUDWidget.h"
#include "UI/WKGASHpBarUserWidget.h"
#include "UI/WKGASWidgetComponent.h"

UWKHUDWidget::UWKHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("UWKHUDWidget::UWKHUDWidget"));
}

void UWKHUDWidget::InitHUDWidget()
{
	UE_LOG(LogTemp, Log, TEXT("UWKHUDWidget::InitHUDWidget"));
}

void UWKHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Log, TEXT("UWKHUDWidget::NativeConstruct"));
	HUDHpBar = Cast<UWKGASHpBarUserWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HUDHpBar);

	if (HUDHpBarComponent)
	{
		HUDHpBarComponent->SetWidget(HUDHpBar);
		HUDHpBarComponent->InitGASWidget();
	}
}
