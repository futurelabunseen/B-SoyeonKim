// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKPlayerController.h"
#include "UI/WKHUDWidget.h"
#include "UI/WKHUD.h"
#include "Game/WKGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AWKPlayerController::AWKPlayerController()
{
	static ConstructorHelpers::FClassFinder<UWKHUDWidget> ABHUDWidgetRef(TEXT("/Game/WarKing/UI/WBP_WKHUD.WBP_WKHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		WKHUDWidgetClass = ABHUDWidgetRef.Class;
	}

	HUDHpBarComponent = CreateDefaultSubobject<UWKGASWidgetComponent>(TEXT("HUDWidget"));
}

void AWKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!WKHUD)
	{
		WKHUD = Cast<AWKHUD>(GetHUD());
	}
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
	TimeLeft = MatchTime - GetServerTime();

	SetHUDMatchCountdown(TimeLeft);
}

void AWKPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	OnRep_MatchState();
}

void AWKPlayerController::HandleMatchHasStarted()
{
	//GameStart
}

void AWKPlayerController::HandleCooldown()
{
	//Cooldown

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

void AWKPlayerController::ClientJoinMidgame_Implementation(float Match)
{
	MatchTime = Match;
}

void AWKPlayerController::ServerCheckMatchState_Implementation()
{
	AWKGameMode* WKGameMode = Cast<AWKGameMode>(UGameplayStatics::GetGameMode(this));
	if (WKGameMode)
	{
		MatchTime = WKGameMode->GetMatchTime();
		ClientJoinMidgame(MatchTime);
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

void AWKPlayerController::InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS, UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS)
{
	//WKHUD = Cast<AWKHUD>(GetHUD());
	if (WKHUD)
	{
		WKHUD->InitOverlay(Player_ASC, Player_AS, Game_ASC, Game_AS);
	}
}


