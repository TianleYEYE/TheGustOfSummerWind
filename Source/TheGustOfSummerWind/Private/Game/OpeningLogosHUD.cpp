// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OpeningLogosHUD.h"

void AOpeningLogosHUD::BeginPlay()
{
	Super::BeginPlay();

	OpeningLogos = CreateWidget<USW_UIBase>(GetOwningPlayerController(),OpeningLogosClass);
	OpeningLogos->AddToViewport();

	OpeningLogos->BlueprintInializeWidget();
}
