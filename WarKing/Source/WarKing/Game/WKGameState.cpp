// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameState.h"
#include "WarKing.h"
//
//AWKGameState::AWKGameState()
//{
//	
//
//}

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

void AWKGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetHUDTime();
}

void AWKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AWKGameState::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());

	//TimeLeft = GetWorld()->GetTime();
	//SetHUDMatchCountdown();
}

void AWKGameState::SetHUDMatchCountdown(const float CountdownTime)
{
}
