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

	void OnLobbyStart();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AWKLobbyHUD> WKLobbyHUD;
};
