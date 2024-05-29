// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/WKUserWidget.h"
#include "WKUserOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKUserOverlayWidget : public UWKUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameTimerText;

};
