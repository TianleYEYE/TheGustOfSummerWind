// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_UICollection.h"

void USW_UICollection::NativeConstruct()
{
	Super::NativeConstruct();
	SetupWidgets();
}

void USW_UICollection::SetupWidgets()
{
	if (UISwitcher)
	{
		//设置BP_NewGame的父级switcher，用来跳转到InGameUI
		BP_MenuUI->BP_ButtonOfInMenu->BP_NewGame->SetParentSwitcher(UISwitcher);
		//设置BP_Continue的父级switcher，用来跳转到ContinueUI
		//BP_MenuUI->BP_ButtonOfInMenu->BP_Continue->SetParentSwitcher(UISwitcher);
		//设置BP_Album的父级switcher，用来跳转到AlbumUI
		BP_MenuUI->BP_ButtonOfInMenu->BP_Album->SetParentSwitcher(UISwitcher);
		//设置BP_Setting的父级switcher，用来跳转到SettingUI
		BP_MenuUI->BP_ButtonOfInMenu->BP_Setting->SetParentSwitcher(UISwitcher);
	}
}

