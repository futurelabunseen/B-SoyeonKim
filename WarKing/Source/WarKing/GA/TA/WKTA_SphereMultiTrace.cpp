// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/WKTA_SphereMultiTrace.h"
#include "Abilities/GameplayAbility.h"
#include "WarKing.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/WKCharacterSkillAttributeSet.h"
#include "Physics/WKCollision.h"
#include "DrawDebugHelpers.h"

FGameplayAbilityTargetDataHandle AWKTA_SphereMultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		
		UE_LOG(LogWKGAS, Log, TEXT("[%s] ASC not found!"), SourceActor->GetOwner()->GetRemoteRole());
		return FGameplayAbilityTargetDataHandle();
	}
	
	const UWKCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UWKCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		UE_LOG(LogWKGAS, Error, TEXT("SkillAttribute not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = SkillAttribute->GetSkillRange();

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AWKTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_WKACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}

	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}

#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}