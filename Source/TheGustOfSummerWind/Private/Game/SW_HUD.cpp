// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_HUD.h"

#include "UI/ViewModel/MVVM_LoadScreen.h"


void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();


	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this,LoadScreenViewModelClass);
	UICollection=CreateWidget<USW_UICollection>(GetOwningPlayerController(),UICollectionClass);
	
	ContinueUI = UICollection->BP_ContinueUI;
	UICollection->AddToViewport();
	InitializeUICollection();

	
	
}

void ASW_HUD::InitializeUICollection()
{
		// 初始化UICollection
		OnUICollectionInitialized.Broadcast();
}



