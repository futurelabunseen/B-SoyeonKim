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

void AWKGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ensure(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);
	
	
		// Init Effect Setting
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> GameplayEffect : StartEffects)
		{
			FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle =
					ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
			}
		}
	}
}

UAbilitySystemComponent* AWKGameState::GetAbilitySystemComponent() const
{
	return ASC;
}

UWKGameAttributeSet* AWKGameState::GetAttributeSet() const
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
