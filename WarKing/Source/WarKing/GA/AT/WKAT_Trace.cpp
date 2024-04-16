// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/WKAT_Trace.h"
#include "GA/TA/WKTA_Trace.h"
#include "AbilitySystemComponent.h"

UWKAT_Trace::UWKAT_Trace()
{
}

UWKAT_Trace* UWKAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AWKTA_Trace> TargetActorClass)
{
	UWKAT_Trace* NewTask = NewAbilityTask<UWKAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UWKAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UWKAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UWKAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AWKTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);

		// Delegate CallBackFunc Bind 
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UWKAT_Trace::OnTargetDataReadyCallback);
	}
}

void UWKAT_Trace::FinalizeTargetActor()
{
	// 최종 확인을 위한 몇가지 작업

	// 현재 ASC를 가져옴
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		// 최종 Transform을 ASC의 Avatar의 Transform으로 바꿔치기
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();

		if (SpawnedTargetActor)
		{
			SpawnedTargetActor->FinishSpawning(SpawnTransform);

			// 오류
			ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
			SpawnedTargetActor->StartTargeting(Ability);

			// TA의 ConfirmTargetingAndContinue가 실행됨
			SpawnedTargetActor->ConfirmTargeting();
		}	
	}
}

void UWKAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	// 내가 생성한 Actor로 부터 Broadcast를 받아서 확인이 끝났을 때 Complete
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
