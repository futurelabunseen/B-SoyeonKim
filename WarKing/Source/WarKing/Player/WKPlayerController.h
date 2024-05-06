// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WKPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWKPlayerController();

	void InitOverlay();

protected:
	virtual void BeginPlay() override;


// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UWKHUDWidget> WKHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UWKHUDWidget> WKHUDWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWKGASWidgetComponent> HUDHpBarComponent;
};
