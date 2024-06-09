// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKLobbyGameMode.h"
#include "GameFramework/GameState.h"
#include "Game/WKLobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/WKLobbyPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerSessionsSubsystem.h"

void AWKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//AWKLobbyPlayerController* WKPlayerController = Cast<AWKLobbyPlayerController>(NewPlayer);

	//if (WKPlayerController)
	//{
	//	WKPlayerController->OnLobbyStart();
	//}

	// 게임에 접속한 Player 갯수 확인
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UE_LOG(LogTemp, Log, TEXT("AWKLobbyGameMode::PostLogin"));
	// TODO : Ready 상태 체크
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

void AWKLobbyGameMode::StartReady()
{
	AWKLobbyGameState* WKLobbyState = Cast<AWKLobbyGameState>(UGameplayStatics::GetGameState(this));
	if (WKLobbyState)
	{
		for (auto PState : WKLobbyState->PlayerArray)
		{
			APlayerState* PlayerState = PState.Get();
			if (PlayerState)
			{
				PlayerState->GetPlayerName();
			}

		/*	AWKGASPlayerState* WKPlayerState = Cast<AWKGASPlayerState>(PState.Get());
			UAbilitySystemComponent* PSASC = WKLobbyState->GetAbilitySystemComponent();
			if (WKPlayerState && WKPlayerState->GetTeam() == WKTAG_GAME_TEAM_NONE)
			{
				if (WKLobbyState->BlueTeam.Num() >= WKLobbyState->RedTeam.Num())
				{
					WKLobbyState->RedTeam.AddUnique(WKPlayerState);
					WKPlayerState->SetTeam(WKTAG_GAME_TEAM_RED);
				}
				else
				{
					WKLobbyState->BlueTeam.AddUnique(WKPlayerState);
					WKPlayerState->SetTeam(WKTAG_GAME_TEAM_BLUE);
				}
			}*/
		}
	}


	GetWorldTimerManager().SetTimer(
		TravelTimer,
		this,
		&ThisClass::ServerTravelGameLevel,
		TraverlTime);
}
