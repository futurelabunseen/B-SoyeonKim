// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Control/WKControl.h"

#include "Components/BoxComponent.h"
#include "Attribute/WKGameAttributeSet.h"
#include "Physics/WKCollision.h"
#include "Game/WKGameState.h"
#include "Tag/WKGameplayTag.h"
#include "WarKing.h"

// Sets default values
AWKControl::AWKControl()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_WKTRIGGER);
	Trigger->SetBoxExtent(FVector(150.0f, 150.0f, 150.0f));

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	//SetReplicates(true);
}

UAbilitySystemComponent* AWKControl::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKControl::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("NotifyActorBeginOverlap"));

	if (ASC)
	{
		ASC->AddLooseGameplayTag(WKTAG_GAME_CONTROL_BLUETEAMPLAYER);
		AttributeSet->SetControlPlayerNumBlue(AttributeSet->GetControlPlayerNumBlue() + 1);
	}	
}

void AWKControl::NotifyActorEndOverlap(AActor* Other)
{
	Super::NotifyActorEndOverlap(Other);

	if (ASC)
	{
		AttributeSet->SetControlPlayerNumBlue(AttributeSet->GetControlPlayerNumBlue() - 1);
		ASC->RemoveLooseGameplayTag(WKTAG_GAME_CONTROL_BLUETEAMPLAYER);
	}
}

// Called when the game starts or when spawned
void AWKControl::BeginPlay()
{
	Super::BeginPlay();
	AWKGameState* CurrentGameState = Cast<AWKGameState>(GetWorld()->GetGameState());

	ASC = CurrentGameState->GetAbilitySystemComponent();
	AttributeSet = CurrentGameState->GetAttributeSet();
}


