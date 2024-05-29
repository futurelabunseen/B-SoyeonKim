// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystemInterface.h"
#include "WKGameState.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKGameState : public AGameStateBase
{
	GENERATED_BODY()


protected:

	virtual void HandleBeginPlay() override;
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	void SetHUDTime();

	void SetHUDMatchCountdown(const float CountdownTime);

public:
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
};
