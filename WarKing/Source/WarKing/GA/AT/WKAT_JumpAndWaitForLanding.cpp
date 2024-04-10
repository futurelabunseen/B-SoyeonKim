// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/WKAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UWKAT_JumpAndWaitForLanding::UWKAT_JumpAndWaitForLanding()
{
}

UWKAT_JumpAndWaitForLanding* UWKAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	// AbilityTask�� ���� Instance ����
	UWKAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UWKAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UWKAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	// �ٴڿ� �������� ���� �⺻ ���� Delegate ȣ��
	Character->LandedDelegate.AddDynamic(this, &UWKAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UWKAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	Super::OnDestroy(AbilityEnded);
}

void UWKAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	// �˷����� �Ⱦ˷� ���� ���� ������ �ľ��� �� ���� -> Ability�� Delegate�� �ٽ� ȣ���Ҷ� üũ�ؾ� �Ѵ�.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
