// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameTimerText;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
