// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKOverlayWidget.h"

void UWKOverlayWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
