// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WKAN_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UWKAN_GASAttackHitCheck::UWKAN_GASAttackHitCheck()
{
	ComboAttackLevel = 1.0f;
}

FString UWKAN_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	//에디터에 보여줄 텍스트
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
			// 내가 지정한 특정한 Actor (ASC) 태그를 넣어서 이벤트 발동 -> 추가적 데이터 지정 가능
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
			UE_LOG(LogTemp, Log, TEXT("Notify SendGameplayEventToActor"));
		}
	}
}
