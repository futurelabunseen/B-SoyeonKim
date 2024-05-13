// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WKCharacterBase.generated.h"

UCLASS()
class WARKING_API AWKCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWKCharacterBase();
	FORCEINLINE virtual class UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }
	FORCEINLINE class UWKComboActionData* GetComboActionData() const { return ComboActionData; }

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWKComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWKGASWidgetComponent> HpBar;

// Dead Section
protected:
	virtual void SetDead();
	void PlayDeadAnimation();

// Stun Section
protected:
	 void SetStun(bool IsStun);
};
