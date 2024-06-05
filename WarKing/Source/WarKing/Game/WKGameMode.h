// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "WKGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	AWKGameMode();

	virtual void Tick(float DeltaTime) override;
	// Network Check Session
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	float LevelStartingTime = 0.f;
public:
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	virtual void HandleMatchHasStarted() override;

private:
	float CountdownTime = 0.f;
public:
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
	FORCEINLINE float GetMatchTime() const { return MatchTime; }
};
