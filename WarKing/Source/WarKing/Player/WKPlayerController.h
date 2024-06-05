// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	void SetHUDTime();

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();
	
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(float Match);

	float SingleTripTime = 0.f;
	float MatchTime = 0.f;

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

	virtual float GetServerTime(); // Synced with server world clock
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UWKHUDWidget> WKHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UWKHUDWidget> WKHUDWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWKGASWidgetComponent> HUDHpBarComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AWKHUD> WKHUD;

	//HUD Update
	void SetHUDMatchCountdown(float CountdownTime);
};
