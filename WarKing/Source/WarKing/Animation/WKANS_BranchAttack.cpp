// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WKANS_BranchAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/WKGameplayTag.h"

void UWKANS_BranchAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), CHARACTER_ACTION_NEXTATTACK, Payload);
}
