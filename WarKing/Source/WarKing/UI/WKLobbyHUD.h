// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WKLobbyHUD.generated.h"

class UWKLobbyWidget;
/**
 * 
 */
UCLASS()
class WARKING_API AWKLobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AWKLobbyHUD();

	void InitOverlay();
private:

	UPROPERTY()
	TObjectPtr<UWKLobbyWidget> LobbyOverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKLobbyWidget> LobbyOverlayWidgetClass;
};
