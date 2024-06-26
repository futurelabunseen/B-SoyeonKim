// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item/WKHealPackSpawner.h"
#include "Components/BoxComponent.h"
#include "Physics/WKCollision.h"
#include "Character/WKCharacterPlayer.h"
#include "Actor/Item/WKHealPack.h"

// Sets default values
AWKHealPackSpawner::AWKHealPackSpawner()
{
	SpawnInterval = 10.0f;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

	RootComponent = Trigger;

	Trigger->SetCollisionProfileName(CPROFILE_WKTRIGGER);
	Trigger->SetBoxExtent(FVector(45.0f, 45.0f, 45.0f));
}

void AWKHealPackSpawner::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	if (CurrentHealPack)
	{
		AWKCharacterPlayer* WKCharacter = Cast<AWKCharacterPlayer>(Other);

		if (WKCharacter)
		{
			SetActorEnableCollision(false);
			CurrentHealPack->OverlapHealPack(Other);
			StartSpawnTimer();
		}
	}
}

// Called when the game starts or when spawned
void AWKHealPackSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnHealPack();
}

void AWKHealPackSpawner::SpawnHealPack()
{
	if (HealPackClass)
	{
		SetActorEnableCollision(true);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		CurrentHealPack = GetWorld()->SpawnActor<AWKHealPack>(HealPackClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void AWKHealPackSpawner::StartSpawnTimer()
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this,
			&AWKHealPackSpawner::SpawnHealPack, SpawnInterval);
	}
}


