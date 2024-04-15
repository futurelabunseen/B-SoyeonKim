// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WKANS_ReadyAttack.h"
#include "AbilitySystemBlueprintLibrary.h"

void UWKANS_ReadyAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(MeshComp->GetOwner(), AttackBeginTags);
}

void UWKANS_ReadyAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(MeshComp->GetOwner(), AttackBeginTags);
}
 