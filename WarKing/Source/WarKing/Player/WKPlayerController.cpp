// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKPlayerController.h"
#include "UI/WKHUDWidget.h"

AWKPlayerController::AWKPlayerController()
{
	static ConstructorHelpers::FClassFinder<UWKHUDWidget> ABHUDWidgetRef(TEXT("/Game/WarKing/UI/WBP_WKHUD.WBP_WKHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		WKHUDWidgetClass = ABHUDWidgetRef.Class;
	}

	HUDHpBarComponent = CreateDefaultSubobject<UWKGASWidgetComponent>(TEXT("HUDWidget"));
}

void AWKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AWKPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
}

void AWKPlayerController::SetHUDTime()
{
}

void AWKPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AWKPlayerController::InitOverlay()
{
	//TODO : Overlay OldCode
	//if (IsLocalController())
	//{
	//	WKHUDWidget = CreateWidget<UWKHUDWidget>(this, WKHUDWidgetClass);
	//	if (WKHUDWidget)
	//	{
	//		WKHUDWidget->HUDHpBarComponent = this->HUDHpBarComponent;
	//	/*	WKHUDWidget->InitHUDWidget();*/
	//		// 애셋을 초기화해서 화면에 띄움
	//		WKHUDWidget->AddToViewport();
	//	}
	//}
}

