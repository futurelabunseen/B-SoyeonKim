// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKLobbyWidget.generated.h"
class UListView;
class UEditableTextBox;
class UButton;
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

	UPROPERTY(meta = (BindWidget))
	UButton* StartBt;
public:
	virtual void NativeConstruct() override;

	void AddToPlayerInfoListView(UObject* ListElementObject);

protected:

	UFUNCTION()
	void OnStartBtClicked();
};
