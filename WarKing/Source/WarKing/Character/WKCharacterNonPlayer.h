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


protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UWKCharacterAttributeSet> AttributeSet;
};