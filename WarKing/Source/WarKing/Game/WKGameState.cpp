// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameState.h"

#include "AbilitySystemComponent.h"
#include "Attribute/WKGameAttributeSet.h"
#include "WarKing.h"

AWKGameState::AWKGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UWKGameAttributeSet>(TEXT("GameAttributeSet"));

	//TODO : 기본값 확인하기
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AWKGameState::GetAbilitySystemComponent() const
{
	return ASC;
}

UAttributeSet* AWKGameState::GetAttributeSet() const
{
	return AttributeSet;
}

void AWKGameState::HandleBeginPlay()
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKGameState::OnRep_ReplicatedHasBegunPlay()
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetHUDTime();
}

void AWKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AWKGameState::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());

	//TimeLeft = GetWorld()->GetTime();
	//SetHUDMatchCountdown();
}

void AWKGameState::SetHUDMatchCountdown(const float CountdownTime)
{
}
