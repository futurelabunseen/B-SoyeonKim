// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WKGASWidgetComponent.h"
#include "UI/WKGASUserWidget.h"

UWKGASWidgetComponent::UWKGASWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWKGASWidgetComponent::InitWidget()
{
	Super::InitWidget();
	InitGASWidget();
}

void UWKGASWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PlayerCameraManager = PC->PlayerCameraManager;
	}
}

void UWKGASWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCameraManager)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(CameraLocation - GetComponentLocation()).Rotator();
		SetWorldRotation(LookAtRotation);
	}
}

void UWKGASWidgetComponent::InitGASWidget()
{
	 GASUserWidget = Cast<UWKGASUserWidget>(GetWidget());

	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
