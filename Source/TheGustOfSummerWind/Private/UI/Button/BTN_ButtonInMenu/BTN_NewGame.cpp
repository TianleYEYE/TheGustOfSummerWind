// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_NewGame.h"

#include "Game/SW_HUD.h"


void UBTN_NewGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_NewGame)
	{
		BTN_NewGame->OnClicked.AddUniqueDynamic(this, &UBTN_NewGame::DelegatedAgentAndPlayAnimation);
	}
}

void UBTN_NewGame::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void UBTN_NewGame::DelegatedAgentAndPlayAnimation()
{
	NewGame.Broadcast();
	if (flashing)
	{
		PlayAnimation(flashing);
	}
}
