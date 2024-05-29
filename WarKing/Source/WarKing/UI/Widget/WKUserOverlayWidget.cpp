// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKUserOverlayWidget.h"

void UWKUserOverlayWidget::SetPlayerWidgetController(UObject* InWidgetController)
{
	PlayerOvelayWidgetController = InWidgetController;
	PlayerWidgetControllerSet();
}

void UWKUserOverlayWidget::SetGameWidgetController(UObject* InWidgetController)
{
	GameOvelayWidgetController = InWidgetController;
	GameWidgetControllerSet();
}
