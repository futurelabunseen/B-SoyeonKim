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
	// ���� Ȯ���� ���� ��� �۾�

	// ���� ASC�� ������
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		// ���� Transform�� ASC�� Avatar�� Transform���� �ٲ�ġ��
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();

		if (SpawnedTargetActor)
		{
			SpawnedTargetActor->FinishSpawning(SpawnTransform);

			// ����
			ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
			SpawnedTargetActor->StartTargeting(Ability);

			// TA�� ConfirmTargetingAndContinue�� �����
			SpawnedTargetActor->ConfirmTargeting();
		}	
	}
}

void UWKAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	// ���� ������ Actor�� ���� Broadcast�� �޾Ƽ� Ȯ���� ������ �� Complete
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
