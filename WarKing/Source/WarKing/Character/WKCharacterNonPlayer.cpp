// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/WKCharacterAttributeSet.h"
#include "Tag/WKGameplayTag.h"

AWKCharacterNonPlayer::AWKCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UWKCharacterAttributeSet>(TEXT("AttributeSet"));
	UE_LOG(LogTemp, Warning, TEXT(" AWKCharacterNonPlayer::AWKCharacterNonPlayer"));
	Level = 1;
}

void AWKCharacterNonPlayer::BeginPlay()
{
	// Server Client ��� ����
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT(" AWKCharacterNonPlayer::BeginPlay"));
	if (AttributeSet)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AWKCharacterNonPlayer::HealthChanged);
	}
}

UAbilitySystemComponent* AWKCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKCharacterNonPlayer::PossessedBy(AController* NewController)
{
	// ���������� ����
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT(" AWKCharacterNonPlayer::PossessedBy"));
	ASC->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	//EffectSpecHandle ����
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}

	ASC->RegisterGameplayTagEvent(WKTAG_CHARACTER_STATE_DEBUFF_STUN,
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);
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

void AWKCharacterNonPlayer::HealthChanged(const FOnAttributeChangeData& Data)
{
	if ((Data.NewValue <= 0.0f) && !ASC->HasMatchingGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD))
	{
		// ������ �ش� Target���� IsDead Tag�� ����
		ASC->AddLooseGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD);
		SetDead();
	}
}

void AWKCharacterNonPlayer::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// TODO: StunTest
	bool bIsCheckStun = NewCount > 0;

	if (!ASC->HasMatchingGameplayTag(WKTAG_CHARACTER_STATE_ISDEAD))
	{
		// Muticast RPC ȣ��
		MulticastSetStun(bIsCheckStun);

		// Server ����
		SetStun(bIsCheckStun);
	}
}

void AWKCharacterNonPlayer::MulticastSetStun_Implementation(bool bIsStun)
{
	// Server ��
	if (!HasAuthority())
		SetStun(bIsStun);
}
