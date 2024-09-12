// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_NewGame.h"

void UBTN_NewGame::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_NewGame->OnClicked.AddDynamic(this,&UBTN_NewGame::DelegatedAgent);
	Flashing.BindDynamic(this,&UBTN_NewGame::FlashingOver);
	BindToAnimationFinished(flashing,Flashing);
	
}

void UBTN_NewGame::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void UBTN_NewGame::DelegatedAgent()
{
	NewGame.Broadcast();
}

void UBTN_NewGame::FlashingOver()
{
	if (ParentSwitcher)
	{
		ParentSwitcher->SetActiveWidgetIndex(1); // Set the index to the desired widget
	}
}

