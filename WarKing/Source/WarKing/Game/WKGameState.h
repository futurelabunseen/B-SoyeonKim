// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystemInterface.h"
#include "WKGameState.generated.h"

class UWKGameAttributeSet;
/**
 * 
 */
UCLASS()
class WARKING_API AWKGameState : public AGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

protected:
	AWKGameState(const FObjectInitializer& ObjectInitializer);

	virtual void HandleBeginPlay() override;
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	void SetHUDTime();

	void SetHUDMatchCountdown(const float CountdownTime);



//GAS Section
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UWKGameAttributeSet> AttributeSet;

public:
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
};
