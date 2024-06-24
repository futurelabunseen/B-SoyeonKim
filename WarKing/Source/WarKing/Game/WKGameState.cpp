// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameState.h"

#include "AbilitySystemComponent.h"
#include "Attribute/WKGameAttributeSet.h"
#include "Game/WKGameMode.h"
#include "WarKing.h"
#include "Tag/WKGameplayTag.h"

AWKGameState::AWKGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UWKGameAttributeSet>(TEXT("GameAttributeSet"));

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

		//if (HasAuthority())
		//{
		//	if (AttributeSet)
		//	{
		//		AGameMode* GameMode = GetWorld()->GetAuthGameMode<AGameMode>();
		//		if (GameMode)
		//		{
		//			AWKGameMode* WKGameMode = Cast<AWKGameMode>(GameMode);
		//			if (WKGameMode)
		//			{
		//				AttributeSet->OnWinnerTeam.AddDynamic();
		//			}
		//		}

		//	}
		//}
	}
}

UAbilitySystemComponent* AWKGameState::GetAbilitySystemComponent() const
{
	return ASC;
}

UAttributeSet* AWKGameState::GetAttributeSet() const
{
	return AttributeSet;
}

float AWKGameState::GetBlueTeamScore() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetControlScoreBlue();
	}
	return 0.0f;
}

float AWKGameState::GetRedTeamScore() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetControlScoreRed();
	}
	return 0.0f;
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

void AWKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
