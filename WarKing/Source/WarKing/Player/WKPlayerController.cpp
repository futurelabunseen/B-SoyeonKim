// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKPlayerController.h"
#include "UI/WKHUDWidget.h"
#include "UI/WKHUD.h"
#include "Game/WKGameMode.h"
#include "Game/WKGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

AWKPlayerController::AWKPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReturnMenuRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_ReturnMenu.IA_ReturnMenu'"));
	if (nullptr != InputActionReturnMenuRef.Object)
	{
		ReturnMenuAction = InputActionReturnMenuRef.Object;
	}
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

void AWKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(ReturnMenuAction, ETriggerEvent::Triggered, this, &ThisClass::ShowReturnToMainMenu);
	}
}


void AWKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWKPlayerController, MatchState);
	DOREPLIFETIME(AWKPlayerController, GameTime);
}
 
void AWKPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	OnRep_MatchState();
}

void AWKPlayerController::OnRespawnState(bool bIsRespawnStart)
{
	if (WKHUD)
	{
		if (bIsRespawnStart)
		{
			WKHUD->SetAnnounceText(TEXT("리스폰 중....."));
			WKHUD->SetAnnounceTimerText(FString());
			WKHUD->SetAnnounceWidgetVisible(ESlateVisibility::Visible);
		}
		else
		{
			if (MatchState == MatchState::InProgress)
			{
				WKHUD->SetAnnounceWidgetVisible(ESlateVisibility::Hidden);
			}		
		}		
	}
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

void AWKPlayerController::OnRep_GameTime()
{
	if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
	{
		SetHUDAnnounceCountdown(GameTime);
	}
	if (MatchState == MatchState::InProgress)
	{
		SetHUDMatchCountdown(GameTime);
	}
}

void AWKPlayerController::SetServerTime()
{
	float TimeLeft = 0.f;	

	if (WKGameMode)
	{
		TimeLeft = WKGameMode->GetCountdownTime();
	}

	GameTime = FMath::FloorToInt(TimeLeft);

	OnRep_GameTime();
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
		WKHUD->SetAnnounceText(FString::Printf(TEXT("곧 게임이 시작됩니다.\n거점을 점령하세요 :")));
	}
}

void AWKPlayerController::ServerCheckMatchState_Implementation()
{
	WKGameMode = Cast<AWKGameMode>(UGameplayStatics::GetGameMode(this));

	if (WKGameMode)
	{
		WarmupTime = WKGameMode->GetWarmupTime();
		MatchTime = WKGameMode->GetMatchTime();
		CooldownTime = WKGameMode->GetCooldownTime();
		LevelStartingTime = WKGameMode->GetLevelStartingTime();
		MatchState = WKGameMode->GetMatchState();

		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
	SetServerTime();

	GetWorldTimerManager().SetTimer(
		TimeTimer,
		this,
		&ThisClass::SetServerTime,
		0.3f, true);
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
		if (CountdownTime <= 0.f)
		{
			WKHUD->SetTimerText(FString());
			return;
		}

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
		if (CountdownTime <= 0.f)
		{
			WKHUD->SetAnnounceTimerText(FString());
			return;
		}
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		WKHUD->SetAnnounceTimerText(CountdownText);
	}
}

void AWKPlayerController::ShowReturnToMainMenu()
{
	if (WKHUD)
	{
		WKHUD->SetReturnMenuOverlayVisibleToggle();
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
			ReturnInfo = FString::Printf(TEXT("레드팀 우승"));
		}
		else if (RedTeamScore < BlueTeamScore)
		{
			ReturnInfo = FString::Printf(TEXT("블루팀 우승"));
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
	if (WKHUD)
	{
		WKHUD->InitOverlay(Player_ASC, Player_AS, Game_ASC, Game_AS);
		WKHUD->AddReturnToMenu();
	}
}


