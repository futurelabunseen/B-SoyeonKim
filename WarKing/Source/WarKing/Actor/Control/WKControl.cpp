// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Control/WKControl.h"

#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/WKGameAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Physics/WKCollision.h"
#include "Game/WKGameState.h"
#include "Tag/WKGameplayTag.h"
#include "UI/WKHUD.h"
#include "UI/Widget/WKUserWidget.h"
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

	ControlWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	ControlWidgetComponent->SetupAttachment(RootComponent);
	ControlWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> ControlWidgetRef(TEXT("/Game/WarKing/UI/WBP_ControlInfo.WBP_ControlInfo_C"));

	if (ControlWidgetRef.Class)
	{
		ControlWidgetComponent->SetWidgetClass(ControlWidgetRef.Class);
		ControlWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		ControlWidgetComponent->SetDrawSize(FVector2D(100.0f, 60.0f));
		ControlWidgetComponent->SetGeometryMode(EWidgetGeometryMode::Plane);
		ControlWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AWKHUD* WKHUD = Cast<AWKHUD>(PlayerController->GetHUD());
		if (WKHUD)
		{
			WKHUD->OnHUDInitialized.AddDynamic(this, &AWKControl::OnHUDInitialized);
		}
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

void AWKControl::OnHUDInitialized(UWKGameWIdgetController* WidgetController)
{
	UWKUserWidget* WKControlWidget = Cast<UWKUserWidget>(ControlWidgetComponent->GetWidget());

	if (WKControlWidget)
		WKControlWidget->SetWidgetController(WidgetController);
}


