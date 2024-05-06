// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKHUD.h"
#include "UI/WKGASUserWidget.h"

void AWKHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UWKGASUserWidget>(Widget);
}
