﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKGASPlayerState.h"
#include "AbilitySystemComponent.h"

AWKGASPlayerState::AWKGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	// 서버에서 클라로 계속 전송이 되어야 하므로 속성 설정
	ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* AWKGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
