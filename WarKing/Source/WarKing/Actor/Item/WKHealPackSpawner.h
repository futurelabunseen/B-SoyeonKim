// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WKHealPackSpawner.generated.h"

UCLASS()
class WARKING_API AWKHealPackSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWKHealPackSpawner();
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AWKHealPack> HealPackClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	TObjectPtr<class AWKHealPack> CurrentHealPack;

	FTimerHandle SpawnTimerHandle;

	void SpawnHealPack();

	void StartSpawnTimer();

};
