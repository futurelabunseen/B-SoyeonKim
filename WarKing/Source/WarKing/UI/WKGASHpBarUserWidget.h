// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WKGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "WKGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGASHpBarUserWidget : public UWKGASUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHpBar();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};