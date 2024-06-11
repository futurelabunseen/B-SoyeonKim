// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKLobbyWidget.h"

#include "UI/Widget/WKPlayerInfoWidget.h"
#include "Components/TextBlock.h"


UWKLobbyWidget::UWKLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UObject> ListElementRef(TEXT("/Game/WarKing/UI/PlayerInfo/WKBP_PlayerInfo_C"));

	if (ListElementRef.Class)
	{
		ListElementClass = ListElementRef.Class;
	}
}

void UWKLobbyWidget::AddToPlayerInfoListView(UObject* ListElementObject)
{
	if (PlayerInfoList)
	{
		//UWKPlayerInfoWidget* NewElement = NewObject<UWKPlayerInfoWidget>(this, ListElementClass);
		//
		//NewElement->NickNameText->SetText(FText::FromString(PlayerName));

		PlayerInfoList->AddItem(ListElementObject);
	}
}
