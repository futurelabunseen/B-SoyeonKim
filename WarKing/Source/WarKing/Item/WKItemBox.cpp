// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WKItemBox.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/WKCollision.h"
#include "AbilitySystemBlueprintLibrary.h"

AWKItemBox::AWKItemBox()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_WKTRIGGER);
	Trigger->SetBoxExtent(FVector(45.0f, 45.0f, 45.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/OtherAssets/Modular_MedievalTown_WI/Meshes/Props/SM_Box.SM_Box'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	//Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetRelativeLocation(FVector(35.0f, 40.0f, -40.0f));
	Mesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	// No Collision
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AWKItemBox::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKItemBox::NotifyActorBeginOverlap(AActor* Other)
{
	// ItemBox Character Overlap
	Super::NotifyActorBeginOverlap(Other);

	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	// 2초 뒤 상자 액터 제거
	SetLifeSpan(2.0f);
}

void AWKItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this, this);
}

void AWKItemBox::ApplyEffectToTarget(AActor* Target)
{
	// 전역 가스 객체에서 해당 액터에 설정된 ASC를 가져온다.
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

void AWKItemBox::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}
