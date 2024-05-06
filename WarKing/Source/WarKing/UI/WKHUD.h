// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WKHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UWKGASUserWidget;
/**
 * 
 */
UCLASS()
class WARKING_API AWKHUD : public AHUD
{
	GENERATED_BODY()
public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	UPROPERTY()
	TObjectPtr<UWKGASUserWidget>  OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKGASUserWidget> OverlayWidgetClass;

	//UPROPERTY()
	//TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
