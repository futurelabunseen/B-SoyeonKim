// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKUserWidget.h"

void UWKUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
