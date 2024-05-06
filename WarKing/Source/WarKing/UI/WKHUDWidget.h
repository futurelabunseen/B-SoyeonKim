// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WKGASUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "WKHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKHUDWidget : public UWKGASUserWidget
{
	GENERATED_BODY()

public:
	UWKHUDWidget(const FObjectInitializer& ObjectInitializer);

	void InitHUDWidget();

protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWKGASHpBarUserWidget> HUDHpBar;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWKGASWidgetComponent> HUDHpBarComponent;
};
