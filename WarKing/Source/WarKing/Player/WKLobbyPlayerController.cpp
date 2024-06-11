// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKLobbyPlayerController.h"

#include "UI/WKLobbyHUD.h"

AWKLobbyPlayerController::AWKLobbyPlayerController()
{
}

void AWKLobbyPlayerController::OnLobbyStart()
{
	UE_LOG(LogTemp, Log, TEXT("AWKLobbyPlayerController::OnLobbyStart"));

}

void AWKLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("AWKLobbyPlayerController::BeginPlay"));

	WKLobbyHUD = Cast<AWKLobbyHUD>(GetHUD());
	if (WKLobbyHUD)
	{
		WKLobbyHUD->InitOverlay();
	}
}