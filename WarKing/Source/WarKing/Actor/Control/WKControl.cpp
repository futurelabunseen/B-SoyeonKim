// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Control/WKControl.h"

#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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
}

UAbilitySystemComponent* AWKControl::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKControl::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	if (Other->HasAuthority())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other);
		WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("NotifyActorBeginOverlap"));

		if (ASC)
		{
			if (TargetASC)
			{
				if (TargetASC->HasMatchingGameplayTag(WKTAG_GAME_TEAM_BLUE))
				{
					AddToBlueTeamPlayerNum(1);
				}
				else if (TargetASC->HasMatchingGameplayTag(WKTAG_GAME_TEAM_RED))
				{
					AddToRedTeamPlayerNum(1);
				}
			}
		}
	}
}

void AWKControl::NotifyActorEndOverlap(AActor* Other)
{
	Super::NotifyActorEndOverlap(Other);

	if (Other->HasAuthority())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other);
		if (ASC)
		{
			if (TargetASC)
			{
				if (TargetASC->HasMatchingGameplayTag(WKTAG_GAME_TEAM_BLUE))
				{
					AddToBlueTeamPlayerNum(-1);
				}
				else if (TargetASC->HasMatchingGameplayTag(WKTAG_GAME_TEAM_RED))
				{
					AddToRedTeamPlayerNum(-1);
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AWKControl::BeginPlay()
{
	Super::BeginPlay();
	AWKGameState* CurrentGameState = Cast<AWKGameState>(GetWorld()->GetGameState());

	ASC = CurrentGameState->GetAbilitySystemComponent();

	UWKGameAttributeSet* WKAttribute = Cast<UWKGameAttributeSet>(CurrentGameState->GetAttributeSet());
	if (WKAttribute)
	{
		AttributeSet = WKAttribute;
	}
}

void AWKControl::AddToBlueTeamPlayerNum(int Count)
{
	AttributeSet->SetControlPlayerNumBlue(AttributeSet->GetControlPlayerNumBlue() + Count);

	// Count -> 양수면 Add, 음수면 Loose
	ASC->UpdateTagMap(WKTAG_GAME_CONTROL_BLUETEAMPLAYER, Count);
}

void AWKControl::AddToRedTeamPlayerNum(int Count)
{
	AttributeSet->SetControlPlayerNumRed(AttributeSet->GetControlPlayerNumRed() + Count);

	// Count -> 양수면 Add, 음수면 Loose
	ASC->UpdateTagMap(WKTAG_GAME_CONTROL_REDTEAMPLAYER, Count);
}


