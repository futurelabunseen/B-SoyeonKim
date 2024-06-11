// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "WKPlayerInfoWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class WARKING_API UWKPlayerInfoWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NickNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> IDText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ReadyImage;
};
