// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_UICollection.h"

void USW_UICollection::NativeConstruct()
{
	Super::NativeConstruct();

	CreateInMenuUIWidget();
	SetupWidgets();
	BP_MenuUI->BP_ButtonOfInMenu->BP_NewGame->BTN_NewGame->OnClicked.AddDynamic(this,&USW_UICollection::SwitchToNewGame);
	BP_MenuUI->BP_ButtonOfInMenu->BP_Continue->BTN_Continue->OnClicked.AddDynamic(this,&USW_UICollection::SwitchToContinue);
	BP_MenuUI->BP_ButtonOfInMenu->BP_Album->BTN_Album->OnClicked.AddDynamic(this,&USW_UICollection::SwitchToAlbum);
	BP_MenuUI->BP_ButtonOfInMenu->BP_Setting->BTN_Setting->OnClicked.AddDynamic(this,&USW_UICollection::SwitchToSetting);
}

void USW_UICollection::SetupWidgets()
{
	if (UISwitcher)
	{
		BP_MenuUI->BP_ButtonOfInMenu->BP_Album->SetParentSwitcher(UISwitcher);
		BP_MenuUI->BP_ButtonOfInMenu->BP_NewGame->SetParentSwitcher(UISwitcher);
		BP_AlbumUI->SetParentSwitcher(UISwitcher);
	}
	
}

void USW_UICollection::SwitchToNewGame()
{
	PlayAnimation(BP_MenuUI->BP_ButtonOfInMenu->BP_NewGame->flashing);
}

void USW_UICollection::SwitchToAlbum()
{
	PlayAnimation(BP_MenuUI->BP_ButtonOfInMenu->BP_Album->flashing);
}

void USW_UICollection::SwitchToSetting()
{
}

void USW_UICollection::SwitchToContinue()
{
}

void USW_UICollection::CreateInMenuUIWidget()
{
	UISwitcher->SetActiveWidgetIndex(0);
}

// void USW_UICollection::CreateInGameUIWidget()
// {
// 	UISwitcher->SetActiveWidgetIndex(1);
// }
//
// void USW_UICollection::CreateContinueUIWidget()
// {
// 	UISwitcher->SetActiveWidgetIndex(2);
// }
//
// void USW_UICollection::CreateAlbumUIWidget()
// {
// 	UISwitcher->SetActiveWidgetIndex(3);
// }
//
// void USW_UICollection::CreateSettingUIWidget()
// {
// 	UISwitcher->SetActiveWidgetIndex(4);
// }
