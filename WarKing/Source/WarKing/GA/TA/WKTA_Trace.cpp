// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/WKTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Physics/WKCollision.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

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
	// Character 가져오기 
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FHitResult OutHitResult;

	// TODO: 후에 Attribute로 옮길 예정
	const float AttackRange = 100.0f;
	const float AttackRadius = 50.f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();

	// 캡슐의 위치부터 시작
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_WKACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;

	// 맞았을 때
	if (HitDetected)
	{
		// HitResilt를 넣어주면 시작지점과 끝 지점에 대한 것을 굳이 따로 설정하지 않아도 알아서 지정해준다.
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}

#endif

	return DataHandle;
}