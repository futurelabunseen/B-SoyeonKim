// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WKCharacterSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class WARKING_API UWKCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	public:
	UWKCharacterSkillAttributeSet();

	ATTRIBUTE_ACCESSORS(UWKCharacterSkillAttributeSet, SkillRange);
	ATTRIBUTE_ACCESSORS(UWKCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UWKCharacterSkillAttributeSet, SkillAttackRate);
	ATTRIBUTE_ACCESSORS(UWKCharacterSkillAttributeSet, MaxSkillAttackRate);
	ATTRIBUTE_ACCESSORS(UWKCharacterSkillAttributeSet, SkillEnergy);
	ATTRIBUTE_ACCESSORS(UWKCharacterSkillAttributeSet, MaxSkillEnergy);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;
};
