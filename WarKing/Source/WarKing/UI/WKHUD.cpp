// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKHUD.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Actor/Control/WKControl.h"
#include "UI/Widget/WKUserOverlayWidget.h"
#include "UI/Widget/WKUserWidget.h"
#include "UI/Widget/WKAnnouncementOverlayWidget.h"
#include "UI/Widget/WKReturnMenuWidget.h"
#include "UI/WidgetController/WKOverlayWidgetController.h"
#include "UI/WidgetController/WKGameWIdgetController.h"

void AWKHUD::BeginPlay()
{
	TArray<AActor*> WKControls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWKControl::StaticClass(), WKControls);
	if (WKControls.Num() > 0)
	{
		AWKControl* WKControl = Cast<AWKControl>(WKControls[0]);
		if (WKControl)
		{
			OnHUDInitialized.AddDynamic(WKControl, &AWKControl::OnHUDInitialized);
		}
	}
}

UWKOverlayWidgetController* AWKHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UWKOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UWKGameWIdgetController* AWKHUD::GetGameOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (GameOverlayWidgetController == nullptr)
	{
		GameOverlayWidgetController = NewObject<UWKGameWIdgetController>(this, GameOverlayWidgetControllerClass);
		GameOverlayWidgetController->SetWidgetControllerParams(WCParams);
		GameOverlayWidgetController->BindCallbacksToDependencies();
	}
	return GameOverlayWidgetController;
}

void AWKHUD::InitOverlay(UAbilitySystemComponent* Player_ASC, UAttributeSet* Player_AS, UAbilitySystemComponent* Game_ASC, UAttributeSet* Game_AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_WKHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_WKHUD"));

	UWKUserOverlayWidget* Widget = CreateWidget<UWKUserOverlayWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UWKUserOverlayWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(Player_ASC, Player_AS);
	UWKOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetPlayerWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	const FWidgetControllerParams GameWidgetControllerParams(Game_ASC, Game_AS);
	UWKGameWIdgetController* GameWidgetController = GetGameOverlayWidgetController(GameWidgetControllerParams);

	OverlayWidget->SetGameWidgetController(GameWidgetController);
	GameWidgetController->BroadcastInitialValues();

	// Control Init Delegate
	OnHUDInitialized.Broadcast(GameWidgetController);


	Widget->AddToViewport();
}

void AWKHUD::SetTimerText(FString CountdownText)
{
	if (OverlayWidget)
	{
		OverlayWidget->Timer->SetText(FText::FromString(CountdownText));
	}
}

void AWKHUD::AddReturnToMenu()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && ReturnMenuWidgetClass)
	{
		ReturnMenuWidget = CreateWidget<UWKReturnMenuWidget>(PlayerController, ReturnMenuWidgetClass);
		ReturnMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		ReturnMenuWidget->AddToViewport();
	}
}

void AWKHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && AnnounceWidgetClass)
	{
		AnnounceWidget = CreateWidget<UWKAnnouncementOverlayWidget>(PlayerController, AnnounceWidgetClass);
		AnnounceWidget->AddToViewport();
	}
}

void AWKHUD::SetAnnounceText(FString AnnounceText)
{
	if (AnnounceWidget)
	{
		AnnounceWidget->AnnounceText->SetText(FText::FromString(AnnounceText));
	}
}

void AWKHUD::SetAnnounceTimerText(FString CountdownText)
{
	if (AnnounceWidget)
	{
		AnnounceWidget->TimeText->SetText(FText::FromString(CountdownText));
	}
}

void AWKHUD::SetAnnounceWidgetVisible(ESlateVisibility Visibility)
{
	if (AnnounceWidget)
	{
		AnnounceWidget->SetVisibility(Visibility);
	}
}

void AWKHUD::SetGameOverlayVisible(ESlateVisibility Visibility)
{
	if (OverlayWidget)
	{
		OverlayWidget->SetVisibility(Visibility);
	}
}

void AWKHUD::SetReturnMenuOverlayVisibleToggle()
{
	if (ReturnMenuWidget)
	{
		if (ReturnMenuWidget->Visibility == ESlateVisibility::Visible)
		{
			ReturnMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ReturnMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
