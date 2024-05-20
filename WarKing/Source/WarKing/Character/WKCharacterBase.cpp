// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/WKCollision.h"
#include "UI/WKGASWidgetComponent.h"
#include "UI/WKGASUserWidget.h"
#include "Animation/WKAnimInstance.h"
#include "WarKing.h"
#include "GameplayTagContainer.h"
#include "Enum/WKCharacterHitType.h"
#include "Enum/WKTEnumToName.h"
#include <EngineUtils.h>
// Sets default values
AWKCharacterBase::AWKCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 100.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_WKCAPSULE);

	// Movements
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/OtherAssets/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	 
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/WarKing/Animation/ABP_WKCharacter.ABP_WKCharacter_C"));

	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/WarKing/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/WarKing/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/WarKing/Animation/AM_HitReact.AM_HitReact'"));
	if (HitReactMontageRef.Object)
	{
		HitReactMontage = HitReactMontageRef.Object;
	}

	// HpBar
	HpBar = CreateDefaultSubobject<UWKGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/WarKing/UI/WBP_HpBar.WBP_HpBar_C"));

	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

EWKHitReactDirection AWKCharacterBase::GetHitReactDirection(const FVector& ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		if (DistanceToRightLeftPlane >= 0)
		{
			return EWKHitReactDirection::Front;
		}
		else
		{
			return EWKHitReactDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EWKHitReactDirection::Right;
		}
		else
		{
			return EWKHitReactDirection::Left;
		}
	}

	return EWKHitReactDirection::Front;
}

void AWKCharacterBase::PlayHitReact_Implementation(EWKHitReactDirection HitDirectionType)
{
	PlayHitReactAnimation(HitDirectionType);
}

bool AWKCharacterBase::PlayHitReact_Validate(EWKHitReactDirection HitDirectionType)
{
	return true;
}

void AWKCharacterBase::PlayHitReactAnimation(EWKHitReactDirection HitDirectionType)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(HitReactMontage, 1.0f);
	AnimInstance->Montage_JumpToSection(*GetEnumDisplayName(HitDirectionType), HitReactMontage);
}

void AWKCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AWKCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AWKCharacterBase::SetStun(bool IsStun)
{
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("SetStun"));
	UWKAnimInstance* AnimInstance = Cast<UWKAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->SetStun(IsStun);

		if (IsStun)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}
		else
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}
