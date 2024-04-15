// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WKGASPlayerState.h"
#include "AbilitySystemComponent.h"

AWKGASPlayerState::AWKGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	// 서버에서 클라로 계속 전송이 되어야 하므로 속성 설정
	ASC->SetIsReplicated(true);

	// Mixed모드로 설정해야 네트워크 대역폭에서 최고의 성능을 얻을 수 있다.
	// Mixed Mode : GameplayTag 및 GameplayCues가 모든 사람들에게 복제됨
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AWKGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
