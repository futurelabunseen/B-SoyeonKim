// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/WKAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UWKAT_JumpAndWaitForLanding::UWKAT_JumpAndWaitForLanding()
{
}

UWKAT_JumpAndWaitForLanding* UWKAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	// AbilityTask에 대한 Instance 생성
	UWKAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UWKAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UWKAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	// 바닥에 떨어지는 순간 기본 제공 Delegate 호출
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
	// 알려줄지 안알려 줄지 현재 설정을 파악할 수 있음 -> Ability를 Delegate로 다시 호출할때 체크해야 한다.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
