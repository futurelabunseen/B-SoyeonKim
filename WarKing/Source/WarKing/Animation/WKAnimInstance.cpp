// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WKAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UWKAnimInstance::UWKAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}
void UWKAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 액터타입이기 때문에 Character인지 알 수 없음 -> 형변환
	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		// 캐릭터 무브먼트에 관한 정보 가져옴
		Movement = Owner->GetCharacterMovement();
	}
}

// 매프레임 Movement 정보를 받아오기
void UWKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 캐릭터 무브먼트 객체로부터 우리가 원하는 값들을 얻어오기
	if (Movement)
	{
		Velocity = Movement->Velocity;

		// 크기는 지면의 속력이기 때문에 XY만 필요 -> 2D 정보
		GroundSpeed = Velocity.Size2D();

		// 지정한 MovingThreashold보다 작은지 검사
		bIsIdle = GroundSpeed < MovingThreshould;

		// 이동 모드의 Falling모드인지만 조사하면 됨
		bIsFalling = Movement->IsFalling();

		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}

void UWKAnimInstance::SetStun(bool IsStun)
{
	bIsStunned = IsStun;
}
