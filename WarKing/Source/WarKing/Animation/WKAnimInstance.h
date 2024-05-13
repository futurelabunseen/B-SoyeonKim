// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	public:
		UWKAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 애니메이션 그래프가 참조할 변수들을 선언

	// 애니메이션 소유 캐릭터 정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// 캐릭터 MovementComponent에 대한 객체 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 값들이라 EditAnywhere 설정 

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	// 땅 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// Idle 상태를 나타내는 bool 
	// bool은 사이즈가 명확하지 않아 unit8:1로 정확히 1bit의 사이즈를 지정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// 움직이는지 쉬는지 확인하기 점검을 위한 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	// 떨어지고 있는지 상태 bool
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// 점프하고 있는지 상태 bool
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// 스턴인지 상태 bool
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsStunned : 1;

	// 점프중인지를 나타내기 위한 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

public:
	void SetStun(bool IsStun);
};
