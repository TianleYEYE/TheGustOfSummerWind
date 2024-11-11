// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_HUD.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"


void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();

	UGameUserSettings::GetGameUserSettings()->LoadSettings();
	

	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this,LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	SystemViewModel = NewObject<UMVVM_System>(this,SystemViewModelClass);
	SystemViewModel ->InitializeLoadSlots();
	
	Menu = CreateWidget<USW_InMenuUI>(GetOwningPlayerController(),MenuClass);
	Menu->AddToViewport();
	Menu->BlueprintInializeWidget();

	SystemViewModel -> LoadData();

	
}


