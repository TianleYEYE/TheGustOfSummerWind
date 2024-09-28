// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_HUD.h"

#include "UI/ViewModel/MVVM_LoadScreen.h"


void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();


	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this,LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();
	
	UICollection=CreateWidget<USW_UICollection>(GetOwningPlayerController(),UICollectionClass);
	UICollection->AddToViewport();
	
	ContinueUI = UICollection->BP_ContinueUI;
	ContinueUI->BlueprintInializeWidget();

	LoadScreenViewModel->LoadData();
	InitializeUICollection();
	
}

void ASW_HUD::InitializeUICollection()
{
		// 初始化UICollection
		OnUICollectionInitialized.Broadcast();
}



