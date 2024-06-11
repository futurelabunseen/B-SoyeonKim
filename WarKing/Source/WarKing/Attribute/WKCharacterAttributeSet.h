// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WKCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

/**
 * 
 */
UCLASS()
class WARKING_API UWKCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWKCharacterAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UWKCharacterAttributeSet, Damage);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	void ResetAttributeSetData();
	//mutable FOutOfHealthDelegate OnOutOfHealth;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	//UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_MoveSpeed)
	//FGameplayAttributeData MoveSpeed;

	bool bOutOfHealth = false;

	friend class UWKGE_AttackDamage;

protected:
	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	/**
	* These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	**/

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	//UFUNCTION()
	//virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
};
