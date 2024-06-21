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
#include "Player/WKGASPlayerState.h"
#include "Tag/WKGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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

	// HpBar
	HpBar = CreateDefaultSubobject<UWKGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());	
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/WarKing/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::World);
		HpBar->SetDrawSize(FVector2D(100.0f, 60.0f));
		HpBar->SetGeometryMode(EWidgetGeometryMode::Plane);
		HpBar->SetDrawAtDesiredSize(false);
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HpBar->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

		// Disable shadows
		HpBar->CastShadow = false;
		HpBar->bCastDynamicShadow = false;
		HpBar->bAffectDynamicIndirectLighting = false;
		HpBar->bAffectDistanceFieldLighting = false;
	}
}

FGameplayTag AWKCharacterBase::GetTeam()
{
	WKPlayerState = WKPlayerState == nullptr ? GetPlayerState<AWKGASPlayerState>() : WKPlayerState;
	if (WKPlayerState == nullptr) return WKTAG_GAME_TEAM_NONE;
	return WKPlayerState->GetTeam();
}

FGameplayTag AWKCharacterBase::GetHitReactDirection(const FVector& ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();

	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());

	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		if (DistanceToRightLeftPlane >= 0)
		{
			return WKTAG_CHARACTER_ACTION_HITREACT_FRONT;
		}
		else
		{
			return WKTAG_CHARACTER_ACTION_HITREACT_BACK;
		}
	}
	else
	{
		if (DistanceToFrontBackPlane >= 0)
		{
			return WKTAG_CHARACTER_ACTION_HITREACT_RIGHT;
		}
		else
		{
			return WKTAG_CHARACTER_ACTION_HITREACT_LEFT;
		}
	}

	return WKTAG_CHARACTER_ACTION_HITREACT_FRONT;
}

void AWKCharacterBase::ServerPlayHitReact_Implementation(FGameplayTag HitDirectionType)
{
	MultiPlayHitReact(HitDirectionType);
}

void AWKCharacterBase::MultiPlayHitReact_Implementation(FGameplayTag HitDirectionType)
{
	FGameplayEventData EventData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, HitDirectionType, EventData);
}

void AWKCharacterBase::SetTeamColor(FGameplayTag Team)
{
	if (GetMesh() == nullptr || RedMaterial == nullptr || BlueMaterial == nullptr) return;

	if (Team == WKTAG_GAME_TEAM_RED)
	{
		GetMesh()->SetSkeletalMesh(RedMaterial);
	}
	else if (Team == WKTAG_GAME_TEAM_BLUE)
	{
		GetMesh()->SetSkeletalMesh(BlueMaterial);
	}
}

void AWKCharacterBase::SetDead()
{
	isDead = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();

	//Ragdoll
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void AWKCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AWKCharacterBase::SetStun(bool IsStun)
{
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
			GetWorldTimerManager().SetTimer(StunTimer, FTimerDelegate::CreateLambda(
				[this]()->void
				{
					if (!isDead)
					{
						// Daad상태 추가
						GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					}	
				}
			), StunCooldownTime, false);
		}
	}
}
