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

AWKTA_Trace::AWKTA_Trace()
{
}

void AWKTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	// Source Actor�� Ability�κ��� ������ �� ����
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
	// Character �������� 
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

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

	FHitResult OutHitResult;

	// TODO: �Ŀ� Attribute�� �ű� ����
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();

	// ĸ���� ��ġ���� ����
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_WKACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;

	// �¾��� ��
	if (HitDetected)
	{
		// HitResilt�� �־��ָ� ���������� �� ������ ���� ���� ���� ���� �������� �ʾƵ� �˾Ƽ� �������ش�.
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