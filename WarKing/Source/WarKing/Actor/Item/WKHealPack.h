// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "WKHealPack.generated.h"

UCLASS()
class WARKING_API AWKHealPack : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWKHealPack();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void OverlapHealPack(AActor* Other);

protected:
	virtual void PostInitializeComponents() override;

	void ApplyEffectToTarget(AActor* Target);
	void InvokeGameplayCue(AActor* Target);


protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<class UNiagaraComponent> Niagara;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;

};
