// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameMode.h"

#include "WarKing.h"
#include "WKGameState.h"
#include "AbilitySystemComponent.h"
#include "Player/WKGASPlayerState.h"
#include "Player/WKPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Tag/WKGameplayTag.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AWKGameMode::AWKGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/WarKing.WKCharacterPlayer"));
	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/WarKing/Blueprint/BP_WKCharacter_Blade.BP_WKCharacter_Blade_C"));
	//if (DefaultPawnClassRef.Class)
	//{
	//	DefaultPawnClass = DefaultPawnClassRef.Class;
	//}

	////클래스 정보가 복제된거라 _C를 붙일 필요는 없음
	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/WarKing.WKPlayerController"));

	//if (PlayerControllerClassRef.Class)
	//{
	//	// DefaultPawnClass
	//	PlayerControllerClass = PlayerControllerClassRef.Class;
	//}
	//
	//GameStateClass = AWKGameState::StaticClass();
	//PlayerStateClass = AWKGASPlayerState::StaticClass();

	bDelayedStart = true;
}

void AWKGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AWKGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("============================================================"));
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	//ErrorMessage = TEXT("Server Is Full");

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

APlayerController* AWKGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));

	return NewPlayerController;
}

void AWKGameMode::PostLogin(APlayerController* NewPlayer)
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);

#pragma region Logging
	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("No Client Connection"));
		}
		else
		{
			for (const auto& Connection : NetDriver->ClientConnections)
			{
				WK_LOG(LogWKNetwork, Log, TEXT("Client Connections: %s"), *Connection->GetName());
			}
		}
	}
	else
	{
		WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
#pragma endregion

	//Team Setting
	AWKGameState* WKGameState = Cast<AWKGameState>(UGameplayStatics::GetGameState(this));

	if (WKGameState)
	{
		AWKGASPlayerState* WKPlayerState = NewPlayer->GetPlayerState<AWKGASPlayerState>();
		if (WKPlayerState && WKPlayerState->GetTeam() == WKTAG_GAME_TEAM_NONE)
		{
			if (WKGameState->BlueTeam.Num() >= WKGameState->RedTeam.Num())
			{
				WKGameState->RedTeam.AddUnique(WKPlayerState);
				WKPlayerState->SetTeam(WKTAG_GAME_TEAM_RED);
			}
			else
			{
				WKGameState->BlueTeam.AddUnique(WKPlayerState);
				WKPlayerState->SetTeam(WKTAG_GAME_TEAM_BLUE);
			}
		}	
	}
}

void AWKGameMode::StartPlay()
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;

		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void AWKGameMode::Logout(AController* Exiting)
{
	AWKGameState* WKGameState = Cast<AWKGameState>(UGameplayStatics::GetGameState(this));
	AWKGASPlayerState* WKPlayerState = Exiting->GetPlayerState<AWKGASPlayerState>();

	if (WKGameState && WKPlayerState)
	{
		if (WKGameState->RedTeam.Contains(WKPlayerState))
		{
			WKGameState->RedTeam.Remove(WKPlayerState);
		}
		if (WKGameState->BlueTeam.Contains(WKPlayerState))
		{
			WKGameState->BlueTeam.Remove(WKPlayerState);
		}
	}
}

void AWKGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AWKPlayerController* WKPlayer = Cast<AWKPlayerController>(*It);
		if (WKPlayer)
		{
			WKPlayer->OnMatchStateSet(MatchState);
		}
	}

	AWKGameState* WKGameState = Cast<AWKGameState>(UGameplayStatics::GetGameState(this));
	
	if (WKGameState)
	{
		UAbilitySystemComponent* PSASC = WKGameState->GetAbilitySystemComponent();

		if (PSASC)
		{
			if (MatchState == MatchState::InProgress)
			{
				PSASC->AddLooseGameplayTag(WKTAG_GAME_STATE_INPROGRESS);
				PSASC->AddReplicatedLooseGameplayTag(WKTAG_GAME_STATE_INPROGRESS);
			}
			else if (MatchState == MatchState::Cooldown)
			{
				PSASC->RemoveLooseGameplayTag(WKTAG_GAME_STATE_INPROGRESS);
				PSASC->RemoveReplicatedLooseGameplayTag(WKTAG_GAME_STATE_INPROGRESS);
			}
		}
	}
}

void AWKGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}

	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void AWKGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	AWKGameState* WKGameState = Cast<AWKGameState>(UGameplayStatics::GetGameState(this));
	if (WKGameState)
	{
		for (auto PState : WKGameState->PlayerArray)
		{
			AWKGASPlayerState* WKPlayerState = Cast<AWKGASPlayerState>(PState.Get());
			UAbilitySystemComponent* PSASC = WKGameState->GetAbilitySystemComponent();
			if (WKPlayerState && WKPlayerState->GetTeam() == WKTAG_GAME_TEAM_NONE)
			{
				if (WKGameState->BlueTeam.Num() >= WKGameState->RedTeam.Num())
				{
					WKGameState->RedTeam.AddUnique(WKPlayerState);
					WKPlayerState->SetTeam(WKTAG_GAME_TEAM_RED);
				}
				else
				{
					WKGameState->BlueTeam.AddUnique(WKPlayerState);
					WKPlayerState->SetTeam(WKTAG_GAME_TEAM_BLUE);
				}
			}
		}
	}
}
