// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_HUD.h"

#include "UI/AlbumUI/SW_AlbumUI.h"


void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();

	
	inMenuUI=CreateWidget<USW_InMenuUI>(GetOwningPlayerController(),InMenuUIClass);
	
	Subsystem =UGameViewportSubsystem::Get(GetWorld());
	
	if(inMenuUI)
	{
		inMenuUI->AddToViewport(1);
	}
	
	Delegated();
}

void ASW_HUD::Delegated()
{
	inMenuUI->BP_ButtonOfInMenu->BP_NewGame->NewGame.AddUObject(this,&ASW_HUD::SwitchToNewGame);
	inMenuUI->BP_ButtonOfInMenu->BP_Album->Album.AddUObject(this,&ASW_HUD::SwitchToAlbum);
	inMenuUI->BP_ButtonOfInMenu->BP_Setting->Setting.AddUObject(this,&ASW_HUD::SwitchToSetting);
	inMenuUI->BP_ButtonOfInMenu->BP_Continue->Continue.AddUObject(this,&ASW_HUD::SwitchToContinue);
}

void ASW_HUD::SwitchToNewGame()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASW_HUD::CreateInGameUIWidget, 1.4f, false);
}

void ASW_HUD::SwitchToAlbum()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASW_HUD::CreateAlbumUIWidget, 1.4f, false);
}

void ASW_HUD::SwitchToSetting()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASW_HUD::CreateSettingUIWidget, 1.4f, false);
}

void ASW_HUD::SwitchToContinue()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASW_HUD::CreateContinueUIWidget, 1.4f, false);
}

void ASW_HUD::CreateInGameUIWidget()
{
	bIsGameStart=true;
	
	inMenuUI->RemoveFromParent();
	InGameUI =CreateWidget<USW_InGameUI>(GetOwningPlayerController(),InGameUIClass);
	
	ViewportSlot = Subsystem->GetWidgetSlot(InGameUI);
	ViewportSlot.ZOrder=1;
	
	Subsystem->AddWidget(InGameUI, ViewportSlot);
}

void ASW_HUD::CreateAlbumUIWidget()
{
	inMenuUI->RemoveFromParent();
	USW_AlbumUI *albumUI=CreateWidget<USW_AlbumUI>(GetOwningPlayerController(),InAlbumUIClass);
	albumUI->AddToViewport(1);
}

void ASW_HUD::CreateSettingUIWidget()
{
	inMenuUI->RemoveFromParent();
	USW_SettingUI *settingUI=CreateWidget<USW_SettingUI>(GetOwningPlayerController(),SettingUIClass);
	settingUI->AddToViewport(1);
}

void ASW_HUD::CreateContinueUIWidget()
{
	inMenuUI->RemoveFromParent();
	USW_ContinueUI *continueUI=CreateWidget<USW_ContinueUI>(GetOwningPlayerController(),ContinuUIClass);
	continueUI->AddToViewport(1);
}