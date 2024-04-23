// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WKAN_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UWKAN_GASAttackHitCheck::UWKAN_GASAttackHitCheck()
{
	ComboAttackLevel = 1.0f;
}

FString UWKAN_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	//�����Ϳ� ������ �ؽ�Ʈ
	return TEXT("GASAttackHitCheck");
}

void UWKAN_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = ComboAttackLevel;
			// ���� ������ Ư���� Actor (ASC) �±׸� �־ �̺�Ʈ �ߵ� -> �߰��� ������ ���� ����
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
			UE_LOG(LogTemp, Log, TEXT("Notify SendGameplayEventToActor"));
		}
	}
}
