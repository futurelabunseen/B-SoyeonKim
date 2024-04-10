// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WKAN_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UWKAN_GASAttackHitCheck::UWKAN_GASAttackHitCheck()
{
}

FString UWKAN_GASAttackHitCheck::GetNotifyName_Implementation() const
{
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
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggetGameplayTag, PayloadData);
		}
	}
}
