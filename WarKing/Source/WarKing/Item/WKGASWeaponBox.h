// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/WKItemBox.h"
#include "WKGASWeaponBox.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API AWKGASWeaponBox : public AWKItemBox
{
	GENERATED_BODY()
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* Other) override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = Event))
	FGameplayTag WeaponEventTag;
};
