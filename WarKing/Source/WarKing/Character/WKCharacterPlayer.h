// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WKCharacterBase.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "WKCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKCharacterPlayer : public AWKCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
private:
	AWKCharacterPlayer();

	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;
protected:
	virtual void BeginPlay() override;
	virtual void OnRep_Owner() override;

	void SetupGASInputComponent();

	void GASInputPressed(int32 InputId);
	void GASInputPressed(const FGameplayTag InputTag);
	void GASInputReleased(int32 InputId);

	void InitGASSetting();
	void SetGASGiveAbility();
	void SetHUD();
	void SetNickNameWidget();
	void ConsoleCommandSetting();

	void SetInitEffects();

	void InitializeInput();

public:
	UFUNCTION()
	bool HasGameplayTag(FGameplayTag Tag) const;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//FORCEINLINE virtual class UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }
	UFUNCTION()
	void Sprint(bool IsSprint);

	UFUNCTION()
	bool GetIsMoving();

	void SetSpawnPoint();

	void SetPlayerDefaults();

	bool GetIsFalling();

	UFUNCTION()
	void ServerSetStopSprint();

	UFUNCTION(Client, Reliable)
	void ClientSetStopSprint();

	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillActionUlt;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	virtual void OnOutOfHealth();

	virtual void SetDead() override;

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void CancelAbilities();
	void RemoveAttackTag();
// GAS Section
protected:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 플레이어에게 부여할 어빌리티의 목록들
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	// Init Effects
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> StartEffects;

	UPROPERTY(EditAnywhere, Category = GAS)
	FGameplayTagContainer  CancelTags;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> SprintCostEffect;

// RPC Section
protected:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetStun(bool bIsStun);

	void MulticastSetStun_Implementation(bool bIsStun);

// Elim Section
protected:
	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 5.f;

	void ElimTimerFinished();
};
