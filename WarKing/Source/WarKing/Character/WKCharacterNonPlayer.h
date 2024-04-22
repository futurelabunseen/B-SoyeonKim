// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WKCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "WKCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKCharacterNonPlayer : public AWKCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
private:

	AWKCharacterNonPlayer();
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetDead() override;

	UFUNCTION()
	virtual void OnOutOfHealth();

	float DeadEventDelayTime = 5.0f;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UWKCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;
};
