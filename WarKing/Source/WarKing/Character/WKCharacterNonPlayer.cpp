// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/WKCharacterAttributeSet.h"

AWKCharacterNonPlayer::AWKCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UWKCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AWKCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}
