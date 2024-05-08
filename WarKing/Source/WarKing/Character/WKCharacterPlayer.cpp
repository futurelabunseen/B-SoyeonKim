// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WKCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/WKGASPlayerState.h"
#include "WarKing.h"
#include "AbilitySystemComponent.h"
#include "UI/WKGASWidgetComponent.h"
#include "Tag/WKGameplayTag.h"
#include "GameplayTagContainer.h"
#include "Attribute/WKCharacterAttributeSet.h"
#include "Player/WKPlayerController.h"
#include "UI/WKHUD.h"

AWKCharacterPlayer::AWKCharacterPlayer()
{	
	// player가 빙의할 때 playerState에서 생성된 ASC값을 대입할 것임
	ASC = nullptr;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/WarKing/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/WarKing/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}
}

void AWKCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("BeginPlay"));
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	// Client의 경우 PlayerController가 없으므로 Skip
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

UAbilitySystemComponent* AWKCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AWKCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("Begin"));

	// Owner 확인 Log
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		WK_LOG(LogWKNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	GASAbilitySetting();
	ConsoleCommandSetting();
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AWKCharacterPlayer::OnRep_Owner()
{
	// 클라는 OnPossess() 호출 X -> PossessedBy 함수 호출되지 않음
	// 클라에서 Owner값이 서버로부터 복제되며 함수 호출	
	WK_LOG(LogWKNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		WK_LOG(LogWKNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("End"));

	ConsoleCommandSetting();
}

void AWKCharacterPlayer::OnRep_PlayerState()
{
	// 클라 전용 
	Super::OnRep_PlayerState();
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("OnRep_PlayerState"));

	// 서버에서 복제한 PlayerState가 존재하고 나서 GAS를 연결해야 하므로 여기서 호출
	GASAbilitySetting();
}

void AWKCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//GAS에서 Binding
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	SetupGASInputComponent();
}

void AWKCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, 0);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, 1);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, WKTAG_CHARACTER_ACTION_ATTACK);

		// Blade Skill Q BlockAttack
		EnhancedInputComponent->BindAction(SkillAction1, ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, WKTAG_CHARACTER_ACTION_SKILL_BLOCKATTACK);
		
		// Blade Skill R AOE
		EnhancedInputComponent->BindAction(SkillActionUlt, ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, WKTAG_CHARACTER_ACTION_SKILL_AOE);
	}
}

void AWKCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AWKCharacterPlayer::GASInputPressed(const FGameplayTag InputTag)
{
	FGameplayTagContainer Container;
	Container.AddTag(InputTag);
	ASC->TryActivateAbilitiesByTag(Container);
}

void AWKCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);

	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AWKCharacterPlayer::GASAbilitySetting()
{
	AWKGASPlayerState* GASPS = GetPlayerState<AWKGASPlayerState>();
	WK_LOG(LogWKNetwork, Log, TEXT("%s"), TEXT("GASAbilitySetting"));

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();

		ASC->InitAbilityActorInfo(GASPS, this);

		// Server에서만 수행
		if (HasAuthority())
		{
			for (const auto& StartAbility : StartAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartAbility);
				ASC->GiveAbility(StartSpec);
			}

			for (const auto& StartInputAbility : StartInputAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
				StartSpec.InputID = StartInputAbility.Key;
				ASC->GiveAbility(StartSpec);
			}

			// Old Code
			/*	const UWKCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UWKCharacterAttributeSet>();
			if (CurrentAttributeSet)
			{
				CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
			}*/
		}

		// Init Effect Setting
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> GameplayEffect : StartEffects)
		{
			FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = 
					ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC.Get());
			}
		}

		GASPS->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

		//TODO : TestCode
		//ASC->AddLooseGameplayTag(WKTAG_EVENT_CHARACTER_ACTION_BLOCKATTACK);

		// HUD Set
		UWKCharacterAttributeSet* CurrentAttributeSet = GASPS->GetAttributeSet();

		if (AWKPlayerController* AuraPlayerController = Cast<AWKPlayerController>(GetController()))
		{
			if (ensure(CurrentAttributeSet))
			{
				if (AWKHUD* WKHUD = Cast<AWKHUD>(AuraPlayerController->GetHUD()))
				{
					WKHUD->InitOverlay(AuraPlayerController, GASPS, ASC, CurrentAttributeSet);
				}
			}	
		}
	}

	// Widget 초기화 작업
	HpBar->InitGASWidget();
}

void AWKCharacterPlayer::ConsoleCommandSetting()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetOwner());

	if (ensure(PlayerController))
	{
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

bool AWKCharacterPlayer::HasGameplayTag(FGameplayTag Tag) const
{
	if (IsValid(ASC))
		return ASC->HasMatchingGameplayTag(Tag);
	else
		return false;
}

void AWKCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AWKCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AWKCharacterPlayer::Sprint(bool IsSprint)
{
	if (IsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0;
	}
}

void AWKCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}

void AWKCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}