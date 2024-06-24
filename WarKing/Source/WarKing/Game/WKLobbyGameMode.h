// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "WKLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditDefaultsOnly)
	int32 GameStartPlayerNum = 2;

	UPROPERTY(EditAnywhere)
	float TraverlTime = 5.0f;
	float LevelStartingTime = 0.f;

	UPROPERTY()
	class AWKLobbyPlayerController* LocalPlayerController;

	void StartReady();

private:
	void ServerTravelGameLevel();
	void Countdown();

	FTimerHandle TravelTimer;
};
