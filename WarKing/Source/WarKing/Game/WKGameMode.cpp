// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WKGameMode.h"

AWKGameMode::AWKGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/WarKing.WKCharacterPlayer"));
	if (ThirdPersonClassRef.Class)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	//클래스 정보가 복제된거라 _C를 붙일 필요는 없음
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/WarKing.WKPlayerController"));

	if (PlayerControllerClassRef.Class)
	{
		// DefaultPawnClass
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}