// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/WKWidgetController.h"

void UWKWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}
void UWKWidgetController::BroadcastInitialValues()
{
}

void UWKWidgetController::BindCallbacksToDependencies()
{
}


