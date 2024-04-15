// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/WKGASPlayerState.h"
#include "EnhancedInputComponent.h"
#include "WarKing.h"
#include "Tag/WKGameplayTag.h"
#include "GameplayTagContainer.h"

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
	// 서버 전용
	//서버에서는 PossessedBy 함수 호출 -> 빙의 
	Super::PossessedBy(NewController);
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	// Owner 확인 Log
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		WK_LOG(LogWKNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	GASAbilitySetting();
	ConsoleCommandSetting();
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKGASCharacterPlayer::OnRep_Owner()
{
	// 클라는 OnPossess() 호출 X -> PossessedBy 함수 호출되지 않음
	// 클라에서 Owner값이 서버로부터 복제되며 함수 호출	
	WK_LOG(LogWKNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		WK_LOG(LogWKNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));

	ConsoleCommandSetting();
}

void AWKGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("SetupPlayerInputComponent"));
	SetupGASInputComponent();
}

void AWKGASCharacterPlayer::OnRep_PlayerState()
{
	// 클라 전용 
	Super::OnRep_PlayerState();
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("OnRep_PlayerState"));

	// 서버에서 복제한 PlayerState가 존재하고 나서 GAS를 연결해야 하므로 여기서 호출
	GASAbilitySetting();
}

void AWKGASCharacterPlayer::GASAbilitySetting()
{	
	AWKGASPlayerState* GASPS = GetPlayerState<AWKGASPlayerState>();
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("GASAbilitySetting"));

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ensure(ASC);
		ASC->InitAbilityActorInfo(GASPS, this);
		
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();
	}
}

void AWKGASCharacterPlayer::ConsoleCommandSetting()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetOwner());

	if (ensure(PlayerController))
	{
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

bool AWKGASCharacterPlayer::HasGameplayTag(FGameplayTag Tag) const
{
	if (IsValid(ASC))
		return ASC->HasMatchingGameplayTag(Tag);
	else
		return false;
}

void AWKGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWKGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWKGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AWKGASCharacterPlayer::GASInputPressed, CHARACTER_ACTION_ATTACK);
	}
}

void AWKGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AWKGASCharacterPlayer::GASInputPressed(const FGameplayTag InputTag)
{
	FGameplayTagContainer Container;
	Container.AddTag(InputTag);
	ASC->TryActivateAbilitiesByTag(Container);
}

void AWKGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}


