// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKLobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UI/WKLobbyHUD.h"
#include "UI/Widget/WKLobbyWidget.h"


AWKLobbyPlayerController::AWKLobbyPlayerController()
{
}

void AWKLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWKLobbyPlayerController, GameTime);
	DOREPLIFETIME(AWKLobbyPlayerController, bIsStartCount);
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

		if (HasAuthority())
		{
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
		}
	}
}

void AWKLobbyPlayerController::OnRep_GameTime()
{
	if (IsLocalController())
	{
		if (WKLobbyHUD)
		{
			FString CountdownText = FString::Printf(TEXT("%02d"), GameTime);

			if (GameTime <= 0.f)
			{
				WKLobbyHUD->SetTimerText(FString());
			}
			WKLobbyHUD->SetTimerText(FString::Printf(TEXT("%d"), GameTime));
		}
	}
}

void AWKLobbyPlayerController::OnRep_IsStartCount()
{
	if (IsLocalController())
	{
		if (bIsStartCount == 1)
		{
			if (WKLobbyHUD)
			{
				WKLobbyHUD->SetTimerVisible(ESlateVisibility::Visible);
			}
		}
	}
}
