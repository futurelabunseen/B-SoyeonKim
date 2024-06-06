// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKPlayerController.h"
#include "UI/WKHUDWidget.h"
#include "UI/WKHUD.h"
#include "Game/WKGameMode.h"
#include "Game/WKGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AWKPlayerController::AWKPlayerController()
{

}

void AWKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!WKHUD)
	{
		WKHUD = Cast<AWKHUD>(GetHUD());
	}

	BlockPlayerInput(false);
	ServerCheckMatchState();
}

void AWKPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
}

void AWKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWKPlayerController, MatchState);
}
 
void AWKPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;

	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnounceCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}
	CountdownInt = SecondsLeft;
}

void AWKPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	OnRep_MatchState();
}

void AWKPlayerController::HandleMatchHasStarted()
{
	//GameStart
	if (WKHUD)
	{
		WKHUD->SetAnnounceWidgetVisible(ESlateVisibility::Hidden);
	}
}

void AWKPlayerController::HandleCooldown()
{
	BlockPlayerInput(true);
	//Cooldown
	if (WKHUD)
	{		
		WKHUD->SetGameOverlayVisible(ESlateVisibility::Hidden);
		WKHUD->SetAnnounceText(GetWinnerText());
		WKHUD->SetAnnounceWidgetVisible(ESlateVisibility::Visible);
	}
}

void AWKPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AWKPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	if (WKHUD && MatchState == MatchState::WaitingToStart)
	{	
		WKHUD->AddAnnouncement();
		WKHUD->SetAnnounceText(FString::Printf(TEXT("New match starts in:")));
	}
}

void AWKPlayerController::ServerCheckMatchState_Implementation()
{
	AWKGameMode* WKGameMode = Cast<AWKGameMode>(UGameplayStatics::GetGameMode(this));
	if (WKGameMode)
	{
		WarmupTime = WKGameMode->GetWarmupTime();
		MatchTime = WKGameMode->GetMatchTime();
		CooldownTime = WKGameMode->GetCooldownTime();
		LevelStartingTime = WKGameMode->GetLevelStartingTime();
		MatchState = WKGameMode->GetMatchState();

		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void AWKPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AWKPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = 0.5f * RoundTripTime;
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void AWKPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

float AWKPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else
	{
		return GetWorld()->GetTimeSeconds() + ClientServerDelta;
	}
}

void AWKPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AWKPlayerController::BlockPlayerInput(bool bBlock)
{
	SetIgnoreMoveInput(bBlock);
	SetIgnoreLookInput(bBlock);

	if(bBlock)
		SetInputMode(FInputModeUIOnly());
	else
		SetInputMode(FInputModeGameOnly());
}

void AWKPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	if (WKHUD)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		WKHUD->SetTimerText(CountdownText);
	}
}

void AWKPlayerController::SetHUDAnnounceCountdown(float CountdownTime)
{
	if (WKHUD)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		WKHUD->SetAnnounceTimerText(CountdownText);
	}
}

FString AWKPlayerController::GetWinnerText()
{
	AWKGameState* WKGameState = Cast<AWKGameState>(GetWorld()->GetGameState());
	FString ReturnInfo = FString();
	if (WKGameState)
	{
		int32 RedTeamScore = FMath::CeilToInt(WKGameState->GetRedTeamScore());
		int32 BlueTeamScore = FMath::CeilToInt(WKGameState->GetBlueTeamScore());

		if (RedTeamScore > BlueTeamScore)
		{
			ReturnInfo = FString::Printf(TEXT("RedTeam Winner!"));
		}
		else if (RedTeamScore < BlueTeamScore)
		{
			ReturnInfo = FString::Printf(TEXT("BlueTeam Winner!"));
		}
		else
		{
			ReturnInfo = FString::Printf(TEXT("Draw"));
		}
		
		ReturnInfo.Append(FString("\n"));
		ReturnInfo.Append(FString::Printf(TEXT("Red : %d%% | Blue : %d%%"), RedTeamScore, BlueTeamScore));
	}

	return ReturnInfo;
}

void AWKPlayerController::InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS, UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS)
{
	//WKHUD = Cast<AWKHUD>(GetHUD());
	if (WKHUD)
	{
		WKHUD->InitOverlay(Player_ASC, Player_AS, Game_ASC, Game_AS);
	}
}


