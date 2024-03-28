// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/WKGASPlayerState.h"

AWKGASCharacterPlayer::AWKGASCharacterPlayer()
{
	// player가 빙의할 때 playerState에서 생성된 ASC값을 대입할 것임
	ASC = nullptr;
}

UAbilitySystemComponent* AWKGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//빙의 타이밍에 ASC를 대입하기 위해
	//TODO : 멀티플레이에서 전달 받기 위해서는 OnRep_PlayerState이벤트 함수에서 구현할 것
	AWKGASPlayerState* GASPS = GetPlayerState<AWKGASPlayerState>();

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);


		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
	}
}
