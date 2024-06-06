// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKLobbyGameMode.h"
#include "GameFramework/GameState.h"
#include "MultiplayerSessionsSubsystem.h"

void AWKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 게임에 접속한 Player 갯수 확인
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/WarKing/Maps/ElvenRuins?listen"));
		}
	}
	
}
