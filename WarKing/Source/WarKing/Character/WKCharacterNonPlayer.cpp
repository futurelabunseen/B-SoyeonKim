// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/WKCharacterAttributeSet.h"

AWKCharacterNonPlayer::AWKCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UWKCharacterAttributeSet>(TEXT("AttributeSet"));

	Level = 1;
}

UAbilitySystemComponent* AWKCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	if (AttributeSet)
	{
		AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
	}


	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	//EffectSpecHandle »ý¼º
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AWKCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AWKCharacterNonPlayer::OnOutOfHealth()
{
	SetDead();
}
