// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "WKGASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AWKGASPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UWKCharacterAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<class UWKCharacterSkillAttributeSet> SkillAttributeSet;
};
