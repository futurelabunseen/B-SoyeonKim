// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagsClasses.h"
#include "WKPlayerStart.generated.h"


/**
 * 
 */
UCLASS()
class WARKING_API AWKPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag TeamTag;
};
