// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WKGameAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWinnerTeam);
/**
 * 
 */
UCLASS()
class WARKING_API UWKGameAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UWKGameAttributeSet();

	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, ControlPlayerNumRed);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, ControlPlayerNumBlue);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, ControlScoreRed);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, ControlScoreBlue);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, MaxControlScore);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, ControlGaugeRed);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, ControlGaugeBlue);
	ATTRIBUTE_ACCESSORS(UWKGameAttributeSet, MaxControlGauge);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWinnerTeam OnWinnerTeam;

protected:

	UAbilitySystemComponent* ASC;

	/// <summary>
	/// RedTeam 거점 인원 수
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlPlayerNum", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_ControlPlayerNumRed)
	FGameplayAttributeData ControlPlayerNumRed;

	/// <summary>
	/// BlueTeam 거점 인원 수
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlPlayerNum", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_ControlPlayerNumBlue)
	FGameplayAttributeData ControlPlayerNumBlue;
	
	/// <summary>
	/// RedTeam 거점 점령 점수
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlScore", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_ControlScoreRed)
	FGameplayAttributeData ControlScoreRed;

	/// <summary>
	/// BlueTeam 거점 점령 점수
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlScore", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_ControlScoreBlue)
	FGameplayAttributeData ControlScoreBlue;

	/// <summary>
	/// 거점 점령 Max 점수
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlScore", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_MaxControlScore)
	FGameplayAttributeData MaxControlScore;

	/// <summary>
	/// RedTeam 거점 점령 달성률
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlGauge", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_ControlGaugeRed)
	FGameplayAttributeData ControlGaugeRed;

	/// <summary>
	/// BlueTeam 거점 점령 달성률
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlGauge", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_ControlGaugeBlue)
	FGameplayAttributeData ControlGaugeBlue;

	/// <summary>
	/// 거점 점령 Max 달성률
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "ControlGauge", Meta = (AllowPrivateAccess = true), ReplicatedUsing = OnRep_MaxControlGauge)
	FGameplayAttributeData MaxControlGauge;

protected:
	UFUNCTION()
	virtual void OnRep_ControlPlayerNumRed(const FGameplayAttributeData& OldControlPlayerNumRed);

	UFUNCTION()
	virtual void OnRep_ControlPlayerNumBlue(const FGameplayAttributeData& OldControlPlayerNumBlue);

	UFUNCTION()
	virtual void OnRep_ControlScoreRed(const FGameplayAttributeData& OldControlScoreRed);

	UFUNCTION()
	virtual void OnRep_ControlScoreBlue(const FGameplayAttributeData& OldControlScoreBlue);

	UFUNCTION()
	virtual void OnRep_MaxControlScore(const FGameplayAttributeData& OldMaxControlScore);

	UFUNCTION()
	virtual void OnRep_ControlGaugeRed(const FGameplayAttributeData& OldControlGaugeRed);

	UFUNCTION()
	virtual void OnRep_ControlGaugeBlue(const FGameplayAttributeData& OldControlGaugeBlue);

	UFUNCTION()
	virtual void OnRep_MaxControlGauge(const FGameplayAttributeData& OldMaxControlGauge);

};
