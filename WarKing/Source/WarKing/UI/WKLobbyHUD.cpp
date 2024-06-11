// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKLobbyHUD.h"

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
	}
}

void AWKLobbyHUD::AddPlayerInfo(UObject* ListElementObject)
{
	if (LobbyOverlayWidget)
	{
		LobbyOverlayWidget->AddToPlayerInfoListView(ListElementObject);
	}
}
