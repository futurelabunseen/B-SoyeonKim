// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKLobbyWidget.h"

#include "UI/Widget/WKPlayerInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Game/WKLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"

UWKLobbyWidget::UWKLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWKLobbyWidget::SetStartBt()
{
	if (StartBt)
	{
		StartBt->SetVisibility(ESlateVisibility::Visible);
		StartBt->OnClicked.AddDynamic(this, &UWKLobbyWidget::OnStartBtClicked);
	}
}

void UWKLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UWKLobbyWidget::OnStartBtClicked()
{
	AWKLobbyGameMode* GameMode = Cast<AWKLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->StartReady();	
	}
}
