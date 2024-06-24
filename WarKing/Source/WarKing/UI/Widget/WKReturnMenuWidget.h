// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKReturnMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class WARKING_API UWKReturnMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* ReturnButton;

	UFUNCTION()
	void ReturnButtonClicked();

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
