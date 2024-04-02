// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameState.h"
#include "WarKing.h"

void AWKGameState::HandleBeginPlay()
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKGameState::OnRep_ReplicatedHasBegunPlay()
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}
