// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameMode.h"
#include "WarKing.h"
#include "WKGameState.h"
#include "Player/WKGASPlayerState.h"

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
}

void AWKGameMode::StartPlay()
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::StartPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}
