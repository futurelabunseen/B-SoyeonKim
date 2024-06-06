// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WKAnnouncementOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKAnnouncementOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnounceText;
};
