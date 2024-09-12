// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_HUD.h"




void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();
	UICollection=CreateWidget<USW_UICollection>(GetOwningPlayerController(),UICollectionClass);
	UICollection->AddToViewport();
}



