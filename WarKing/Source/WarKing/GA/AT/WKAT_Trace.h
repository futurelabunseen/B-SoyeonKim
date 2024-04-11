// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WKAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class WARKING_API UWKAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
public:
	UWKAT_Trace();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWKAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class UWKAT_Trace> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class UWKAT_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AWKTA_Trace> SpawnedTargetActor;
};
