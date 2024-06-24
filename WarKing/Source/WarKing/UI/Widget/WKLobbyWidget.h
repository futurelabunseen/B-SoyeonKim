// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKLobbyWidget.generated.h"


/**
 * 
 */
UCLASS()
class WARKING_API UWKLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWKLobbyWidget(const FObjectInitializer& ObjectInitializer);

	void SetStartBt();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> StartBt;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Timer;
public:
	virtual void NativeConstruct() override;

protected:

	UFUNCTION()
	void OnStartBtClicked();
};
