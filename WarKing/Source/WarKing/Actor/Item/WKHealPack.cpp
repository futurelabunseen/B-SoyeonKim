// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/WKHealPack.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/WKCollision.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AWKHealPack::AWKHealPack()
{
	bReplicates = true;
	SetReplicatingMovement(true);
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));

	Niagara->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/OtherAssets/sA_PickupSet_1/Fx/NiagaraSystems/NS_Pickup_3.NS_Pickup_3'"));
	if (NiagaraSystemRef.Succeeded())
	{
		Niagara->SetAsset(NiagaraSystemRef.Object);
	}
	
	Niagara->SetRelativeLocation(FVector::ZeroVector);
	Niagara->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	Niagara->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AWKHealPack::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKHealPack::OverlapHealPack(AActor * Other)
{
	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);

	Niagara->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	// 2초 뒤 상자 액터 제거
	SetLifeSpan(0.1f);
}

void AWKHealPack::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this, this);
}

void AWKHealPack::ApplyEffectToTarget(AActor* Target)
{	// 전역 가스 객체에서 해당 액터에 설정된 ASC를 가져온다.
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AWKHealPack::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}

