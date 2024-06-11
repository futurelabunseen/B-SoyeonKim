// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKLobbyWidget.generated.h"
class UListView;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class WARKING_API UWKLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWKLobbyWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UListView> PlayerInfoList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> NickNameTextBox;

	UPROPERTY()
	TSubclassOf<class UWKPlayerInfoWidget> ListElementClass;

public:

	void AddToPlayerInfoListView(UObject* ListElementObject);
};
