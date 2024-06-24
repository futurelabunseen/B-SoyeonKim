// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKLobbyHUD.h"

#include "Components/TextBlock.h"
#include "UI/Widget/WKLobbyWidget.h"

AWKLobbyHUD::AWKLobbyHUD()
{
}

void AWKLobbyHUD::InitOverlay()
{
	checkf(LobbyOverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_WKLobbyHUD"));

	UWKLobbyWidget* Widget = CreateWidget<UWKLobbyWidget>(GetWorld(), LobbyOverlayWidgetClass);
	
	if (Widget)
	{
		LobbyOverlayWidget = Cast<UWKLobbyWidget>(Widget);
		LobbyOverlayWidget->AddToViewport();

		if (GetOwner()->HasAuthority())
		{
			LobbyOverlayWidget->SetStartBt();
		}
	}
}

void AWKLobbyHUD::SetTimerText(FString TimerText)
{
	if (LobbyOverlayWidget)
	{
		LobbyOverlayWidget->Timer->SetText(FText::FromString(TimerText));
	}
}

void AWKLobbyHUD::SetTimerVisible(ESlateVisibility Visible)
{
	if (LobbyOverlayWidget)
	{
		LobbyOverlayWidget->Timer->SetVisibility(Visible);
	}
}
