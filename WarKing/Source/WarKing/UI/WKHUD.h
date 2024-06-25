// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WidgetController/WKGameWIdgetController.h"
#include "WKHUD.generated.h"

class UWKOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UWKUserOverlayWidget;
class UWKAnnouncementOverlayWidget;
class UWKUserWidget;
struct FWidgetControllerParams;
class UWKReturnMenuWidget;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDInitialized, UWKGameWIdgetController*, WidgetController);
/**
 * 
 */
UCLASS()
class WARKING_API AWKHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHUDInitialized OnHUDInitialized;

	virtual void BeginPlay() override;

	UWKOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UWKGameWIdgetController* GetGameOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS,
		UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS);	

	void SetTimerText(FString CountdownText);
	void AddAnnouncement();
	void AddReturnToMenu();
	void SetAnnounceText(FString AnnounceText);
	void SetAnnounceTimerText(FString CountdownText);
	void SetAnnounceWidgetVisible(ESlateVisibility Visibility);
	void SetGameOverlayVisible(ESlateVisibility Visibility);
	void SetReturnMenuOverlayVisible(ESlateVisibility Visibility);
	void SetKeyGuideVisible(ESlateVisibility Visibility);

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

	UPROPERTY()
	TObjectPtr<UWKReturnMenuWidget> ReturnMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWKReturnMenuWidget> ReturnMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> KeyGuideWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> KeyGuideWidgetClass;
};
