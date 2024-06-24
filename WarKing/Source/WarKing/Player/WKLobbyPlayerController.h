// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WKLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWKLobbyPlayerController();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void OnLobbyStart();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AWKLobbyHUD> WKLobbyHUD;

	UPROPERTY(ReplicatedUsing = OnRep_GameTime)
	uint32 GameTime;

	UPROPERTY(ReplicatedUsing = OnRep_IsStartCount)
	uint32 bIsStartCount = 0;


	UFUNCTION()
	void OnRep_GameTime();
protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnRep_IsStartCount();
};
