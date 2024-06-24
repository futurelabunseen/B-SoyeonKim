// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/WKTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Physics/WKCollision.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Attribute/WKCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/WKCharacterBase.h"
#include "Tag/WKGameplayTag.h"

AWKTA_Trace::AWKTA_Trace()
{
}

void AWKTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	// Source Actor는 Ability로부터 가져올 수 있음
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AWKTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AWKTA_Trace::MakeTargetData() const
{
	AWKCharacterBase* SourceCharacter = CastChecked<AWKCharacterBase>(SourceActor);

	//AttributeSet
#pragma region AttributeSet
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("AWKTA_Trace::MakeTargetData() : ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UWKCharacterAttributeSet* AttributeSet = ASC->GetSet<UWKCharacterAttributeSet>();

	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("AWKTA_Trace::MakeTargetData() : AttributeSet not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

#pragma endregion
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, SourceCharacter);
	const FVector Forward = SourceCharacter->GetActorForwardVector();

	// 캡슐의 위치부터 시작
	const FVector Start = SourceCharacter->GetActorLocation() + Forward * SourceCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	FRotator CharacterRotation = SourceCharacter->GetActorRotation();
	FQuat Rotation = FQuat(CharacterRotation + FRotator(0.f, 0.f, 90.f));
	TArray<FHitResult> OutHitResults;

	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		Rotation,
		CCHANNEL_WKACTION,
		FCollisionShape::MakeCapsule(AttackRadius, AttackRange),
		Params
	);

	FGameplayAbilityTargetDataHandle DataHandle;

//#if ENABLE_DRAW_DEBUG
//	if (bShowDebug)
//	{
//		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//		
//		DrawDebugCapsule(GetWorld(),
//			CapsuleOrigin, 
//			AttackRange,
//			AttackRadius, 
//			Rotation,
//			DrawColor,
//			false,
//			5.0f
//		);
//	}
//#endif

	// 맞았을 때
	if (HitDetected)
	{
		for (const FHitResult& OutHitResult : OutHitResults)
		{
			AWKCharacterBase* TargetCharacter = CastChecked<AWKCharacterBase>(OutHitResult.GetActor());

			if (TargetCharacter)
			{
				if (SourceCharacter->GetTeam() != TargetCharacter->GetTeam() || TargetCharacter->GetTeam() == WKTAG_GAME_TEAM_NONE)
				{
					// HitResult를 넣어주면 시작지점과 끝 지점에 대한 것을 굳이 따로 설정하지 않아도 알아서 지정해준다.
	 
					FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
					DataHandle.Add(TargetData);
				}
			}
		}	
	}

	return DataHandle;
}