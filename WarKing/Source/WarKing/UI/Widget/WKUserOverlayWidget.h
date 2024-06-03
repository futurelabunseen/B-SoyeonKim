// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKUserOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKUserOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable)
	void SetGameWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> PlayerOvelayWidgetController;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> GameOvelayWidgetController;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Timer;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerWidgetControllerSet();

	UFUNCTION(BlueprintImplementableEvent)
	void GameWidgetControllerSet();
};
