// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WKGASWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARKING_API UWKGASWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	UWKGASWidgetComponent();
	virtual void InitWidget() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<APlayerCameraManager> PlayerCameraManager;
public:

	void InitGASWidget();
};
