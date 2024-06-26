// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKPlayerController.h"
#include "UI/WKHUDWidget.h"
#include "UI/WKHUD.h"
#include "Game/WKGameMode.h"
#include "Game/WKGameState.h"
#include "Player/WKGASPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tag/WKGameplayTag.h"
#include "Net/UnrealNetwork.h"

AWKPlayerController::AWKPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReturnMenuRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_ReturnMenu.IA_ReturnMenu'"));
	if (nullptr != InputActionReturnMenuRef.Object)
	{
		ReturnMenuAction = InputActionReturnMenuRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> KeyGuideActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_KeyGuide.IA_KeyGuide'"));
	if (nullptr != KeyGuideActionRef.Object)
	{
		KeyGuideAction = KeyGuideActionRef.Object;
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
		EnhancedInputComponent->BindAction(KeyGuideAction, ETriggerEvent::Triggered, this, &ThisClass::ShowKeyGuide);
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
	if (MatchState == MatchState::InProgress)
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
		UE_LOG(LogTemp, Log, TEXT("AWKPlayerController::ShowReturnToMainMenu"));
		if (!bShowMenu)
		{
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
			WKHUD->SetReturnMenuOverlayVisible(ESlateVisibility::Visible);
		}
		else
		{
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
			WKHUD->SetReturnMenuOverlayVisible(ESlateVisibility::Hidden);
		}
		bShowMenu = !bShowMenu;
	}
}

void AWKPlayerController::ShowKeyGuide()
{
	if (WKHUD)
	{
		UE_LOG(LogTemp, Log, TEXT("AWKPlayerController::ShowKeyGuide"));
		if (bShowKeyGuide)
		{
			WKHUD->SetKeyGuideVisible(ESlateVisibility::Visible);
		}
		else
		{
			WKHUD->SetKeyGuideVisible(ESlateVisibility::Hidden);
		}
		bShowKeyGuide = !bShowKeyGuide;
	}
}

FString AWKPlayerController::GetWinnerText()
{
	AWKGameState* WKGameState = Cast<AWKGameState>(GetWorld()->GetGameState());
	AWKGASPlayerState* WKPlayerState = GetPlayerState<AWKGASPlayerState>();
	FString ReturnInfo = FString();
	if (WKGameState && WKPlayerState)
	{
		int32 RedTeamScore = FMath::CeilToInt(WKGameState->GetRedTeamScore());
		int32 BlueTeamScore = FMath::CeilToInt(WKGameState->GetBlueTeamScore());
		
		FGameplayTag MyTeam = WKPlayerState->GetTeam();
		FGameplayTag WinTeam = WKTAG_GAME_TEAM_NONE;

		if (RedTeamScore > BlueTeamScore)
		{
			WinTeam = WKTAG_GAME_TEAM_RED;
		}
		else if (RedTeamScore < BlueTeamScore)
		{
			WinTeam = WKTAG_GAME_TEAM_BLUE;
		}

		if (WinTeam == WKTAG_GAME_TEAM_NONE)
		{
			ReturnInfo = FString::Printf(TEXT("Draw"));
		}
		else
		{
			if (MyTeam == WKTAG_GAME_TEAM_BLUE)
			{
				ReturnInfo = FString::Printf(TEXT("블루팀 "));
			}
			else if (MyTeam == WKTAG_GAME_TEAM_RED)
			{
				ReturnInfo = FString::Printf(TEXT("레드팀 "));
			}

			if (MyTeam == WinTeam)
			{
				ReturnInfo.Append(FString(TEXT("승리")));
			}
			else
			{
				ReturnInfo.Append(FString(TEXT("패배")));
			}
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


