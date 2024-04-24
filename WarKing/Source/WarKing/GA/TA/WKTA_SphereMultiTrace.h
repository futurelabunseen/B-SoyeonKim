// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/WKTA_Trace.h"
#include "WKTA_SphereMultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKTA_SphereMultiTrace : public AWKTA_Trace
{
	GENERATED_BODY()
	
protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
