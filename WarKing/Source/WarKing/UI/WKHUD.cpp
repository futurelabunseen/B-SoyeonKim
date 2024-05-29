// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKHUD.h"

#include "Components/TextBlock.h"
#include "UI/Widget/WKUserOverlayWidget.h"
#include "UI/WidgetController/WKOverlayWidgetController.h"
#include "UI/WidgetController/WKGameWIdgetController.h"

UWKOverlayWidgetController* AWKHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UWKOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UWKGameWIdgetController* AWKHUD::GetGameOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (GameOverlayWidgetController == nullptr)
	{
		GameOverlayWidgetController = NewObject<UWKGameWIdgetController>(this, GameOverlayWidgetControllerClass);
		GameOverlayWidgetController->SetWidgetControllerParams(WCParams);
		GameOverlayWidgetController->BindCallbacksToDependencies();
	}
	return GameOverlayWidgetController;
}

void AWKHUD::InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS, UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_WKHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_WKHUD"));

	UWKUserOverlayWidget* Widget = CreateWidget<UWKUserOverlayWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UWKUserOverlayWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(Player_ASC, Player_AS);
	UWKOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetPlayerWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	const FWidgetControllerParams GameWidgetControllerParams(Game_ASC, Game_AS);
	UWKGameWIdgetController* GameWidgetController = GetGameOverlayWidgetController(GameWidgetControllerParams);

	OverlayWidget->SetGameWidgetController(GameWidgetController);
	GameWidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
