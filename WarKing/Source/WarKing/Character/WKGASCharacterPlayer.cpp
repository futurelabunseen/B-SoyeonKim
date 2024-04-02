// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/WKGASPlayerState.h"
#include "EnhancedInputComponent.h"
#include "WarKing.h"

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

	//빙의 타이밍에 ASC를 대입하기 위해
	//TODO : 멀티플레이에서 전달 받기 위해서는 OnRep_PlayerState이벤트 함수에서 구현할 것
	AWKGASPlayerState* GASPS = GetPlayerState<AWKGASPlayerState>();

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		// 어빌리티의 순서대로 아이디 부여
		int32 InputId = 0;
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			StartSpec.InputID = InputId++;
			ASC->GiveAbility(StartSpec);

			// 서버에서 호출 되는 경우를 위해 SetupPlayerInputComponent()외에 중복 호출
			SetupGASInputComponent();
		}
	}

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKGASCharacterPlayer::OnRep_Owner()
{
	//클라에서 Owner값이 서버로부터 복제되며 함수 호출
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
}

void AWKGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();
}

void AWKGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWKGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWKGASCharacterPlayer::GASInputReleased, 0);
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
