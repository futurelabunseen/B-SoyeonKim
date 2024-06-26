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
	// Server Client 모두 수행
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
	// 서버에서만 수행
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT(" AWKCharacterNonPlayer::PossessedBy"));
	ASC->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	//EffectSpecHandle 생성
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
		// 죽으면 해당 Target에는 IsDead Tag가 부착
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
		// Muticast RPC 호출
		MulticastSetStun(bIsCheckStun);

		// Server 수행
		SetStun(bIsCheckStun);
	}
}

void AWKCharacterNonPlayer::MulticastSetStun_Implementation(bool bIsStun)
{
	// Server 외
	if (!HasAuthority())
		SetStun(bIsStun);
}
