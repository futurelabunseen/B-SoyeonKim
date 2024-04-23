// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WKGASWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"

void AWKGASWeaponBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, WeaponEventTag, FGameplayEventData());
}
