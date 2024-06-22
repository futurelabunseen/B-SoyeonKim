// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WKHUD.generated.h"

class UWKOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UWKUserOverlayWidget;
class UWKAnnouncementOverlayWidget;
class UWKGameWIdgetController;
class UWKUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class WARKING_API AWKHUD : public AHUD
{
	GENERATED_BODY()
public:

	UWKOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UWKGameWIdgetController* GetGameOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS,
		UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS);	

	void SetControlWidget(UWKUserWidget* InGameControlWidget);

	void SetTimerText(FString CountdownText);
	void AddAnnouncement();
	void SetAnnounceText(FString AnnounceText);
	void SetAnnounceTimerText(FString CountdownText);
	void SetAnnounceWidgetVisible(ESlateVisibility Visibility);
	void SetGameOverlayVisible(ESlateVisibility Visibility);

	UPROPERTY()
	TObjectPtr<UWKAnnouncementOverlayWidget> AnnounceWidget;

private:
	UPROPERTY()
	TObjectPtr<UWKUserOverlayWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKUserOverlayWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKAnnouncementOverlayWidget> AnnounceWidgetClass;

	UPROPERTY()
	TObjectPtr<UWKOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UWKGameWIdgetController> GameOverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKGameWIdgetController> GameOverlayWidgetControllerClass;

	UPROPERTY(EditAnywhere, Category = Widget)
	TObjectPtr<UWKUserWidget> ControlWidget;
};
