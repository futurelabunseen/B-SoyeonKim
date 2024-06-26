// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKReturnMenuWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Character/WKCharacterPlayer.h"

void UWKReturnMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.AddDynamic(this, &UWKReturnMenuWidget::ReturnButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UWKReturnMenuWidget::OnDestroySession);
		}
	}
}

void UWKReturnMenuWidget::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* FirstPlayerController = World->GetFirstPlayerController();
		if (FirstPlayerController)
		{
			AWKCharacterPlayer* WKCharacter = Cast<AWKCharacterPlayer>(FirstPlayerController->GetPawn());
			if (WKCharacter)
			{
				OnPlayerLeftGame();
			/*	WKCharacter->ServerLeaveGame();
				WKCharacter->OnLeftGame.AddDynamic(this, &UReturnToMainMenu::OnPlayerLeftGame);*/
			}
			else
			{
				ReturnButton->SetIsEnabled(true);
			}
		}
	}
}

void UWKReturnMenuWidget::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UWKReturnMenuWidget::OnPlayerLeftGame()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPlayerLeftGame()"))
		if (MultiplayerSessionsSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("MultiplayerSessionsSubsystem valid"))
				MultiplayerSessionsSubsystem->DestroySession();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MultiplayerSessionsSubsystem not valid. Cannot destroy session."))
				ReturnButton->SetIsEnabled(true);
		}
}
