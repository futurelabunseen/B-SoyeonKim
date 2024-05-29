// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WKHUD.generated.h"

class UWKOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UWKUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class WARKING_API AWKHUD : public AHUD
{
	GENERATED_BODY()
public:

	UWKOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	UPROPERTY()
	TObjectPtr<UWKUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UWKOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKOverlayWidgetController> OverlayWidgetControllerClass;
};
