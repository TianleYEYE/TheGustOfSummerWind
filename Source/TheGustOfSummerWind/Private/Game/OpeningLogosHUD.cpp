// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OpeningLogosHUD.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void AOpeningLogosHUD::BeginPlay()
{
	Super::BeginPlay();

	if (OpeningLogosClass)
	{
		OpeningLogos = CreateWidget<USW_UIBase>(GetOwningPlayerController(), OpeningLogosClass);
		if (OpeningLogos)
		{
			OpeningLogos->AddToViewport();
		}
	}

	if (OpeningLogoDuration <= 0.0f)
	{
		OpenMainMenu();
	}
	else if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(OpenMainMenuTimerHandle, this, &AOpeningLogosHUD::OpenMainMenu, OpeningLogoDuration, false);
	}
}

void AOpeningLogosHUD::OpenMainMenu()
{
	if (!MainMenuLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName);
	}
}
