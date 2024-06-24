// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemInterface.h"
#include "WKPlayerController.generated.h"

class UAttributeSet;
/**
 * 
 */
UCLASS()
class WARKING_API AWKPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWKPlayerController();

	void InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS, UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void OnMatchStateSet(FName State);
	void OnRespawnState(bool bIsRespawnStart);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReturnMenuAction;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void HandleMatchHasStarted();
	void HandleCooldown();

	class AWKGameMode* WKGameMode;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();
	
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);
	
	UPROPERTY(ReplicatedUsing = OnRep_GameTime)
	uint32 GameTime;

	UFUNCTION()
	void OnRep_GameTime();

	FTimerHandle TimeTimer;

	void SetServerTime();

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	float SingleTripTime = 0.f;

	float ClientServerDelta = 0.f; // difference between client and server time

	void BlockPlayerInput(bool bBlock);

	uint32 SecondsLeft;
	bool bShowMenu = false;

// HUD Section
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AWKHUD> WKHUD;

	//HUD Update
	void SetHUDMatchCountdown(float CountdownTime);

	void SetHUDAnnounceCountdown(float Coun);

	void ShowReturnToMainMenu();

	FString GetWinnerText();
};
