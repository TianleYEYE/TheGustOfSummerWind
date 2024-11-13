// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AlbumUI/SW_AlbumUI.h"

#include "Game/SW_HUD.h"



void USW_AlbumUI::NativeConstruct()
{
	Super::NativeConstruct();

	PageSwitcher->SetActiveWidgetIndex(0);
	
	BP_Next->Next.AddUObject(this,&USW_AlbumUI::SwitchToNextPage_2);
	BP_Previous->Previous.AddUObject(this,&USW_AlbumUI::SwitchToNextPage_1);
}

void USW_AlbumUI::SwitchToNextPage_2()
{
	PageSwitcher->SetActiveWidgetIndex(1);
}

void USW_AlbumUI::SwitchToNextPage_1()
{
	PageSwitcher->SetActiveWidgetIndex(0);
}


