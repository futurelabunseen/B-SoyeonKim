// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WKCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "WKGASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKGASCharacterPlayer : public AWKCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
private:
	AWKGASCharacterPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void OnRep_Owner() override;

protected:
	void SetupGASInputComponent();

	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 플레이어에게 부여할 어빌리티의 목록들
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
};
