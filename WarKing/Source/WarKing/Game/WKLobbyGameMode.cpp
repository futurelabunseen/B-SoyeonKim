// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKLobbyGameMode.h"
#include "GameFramework/GameState.h"
#include "Game/WKLobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/WKLobbyPlayerController.h"
#include "UI/WKLobbyHUD.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerSessionsSubsystem.h"

void AWKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (LocalPlayerController)
	{
		if (NewPlayer->IsLocalController())
		{
			AWKLobbyPlayerController* LocallyController = Cast<AWKLobbyPlayerController>(NewPlayer);
			if (LocallyController)
			{
				LocalPlayerController = LocallyController;
			}
		}
	}
	// 게임에 접속한 Player 갯수 확인
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UE_LOG(LogTemp, Log, TEXT("AWKLobbyGameMode::PostLogin"));
	
	if (NumberOfPlayers == GameStartPlayerNum)
	{		
		// Player정보 담기
		StartReady();
	}
}

void AWKLobbyGameMode::ServerTravelGameLevel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/WarKing/Maps/ElvenRuins?listen"));
	}
}

void AWKLobbyGameMode::Countdown()
{
	float CountdownTime = TraverlTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

	UE_LOG(LogTemp, Log ,TEXT("Count"));
	if (CountdownTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(TravelTimer);
		ServerTravelGameLevel();
	}
}

void AWKLobbyGameMode::StartReady()
{
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			TravelTimer,
			this,
			&ThisClass::Countdown,
			0.5f, true);

		LevelStartingTime = GetWorld()->GetTimeSeconds();
	}	
}
