// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WKPlayerInfoWidget.h"

#include "Components/TextBlock.h"

void UWKPlayerInfoWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	//FString* ListObject = Cast<FString>(ListItemObject);

	//if (ListObject)
	//{
	//	NickNameText->SetText(FText::FromString(*ListObject));
	//}
}
