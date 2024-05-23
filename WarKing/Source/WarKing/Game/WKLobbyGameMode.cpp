// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKLobbyGameMode.h"
#include "GameFramework/GameState.h"

void AWKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
}
