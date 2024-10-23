// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_NewGame.h"

#include "Game/SW_HUD.h"


void UBTN_NewGame::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_NewGame->OnClicked.AddDynamic(this,&UBTN_NewGame::DelegatedAgentAndPlayAnimation);
	Flashing.BindDynamic(this,&UBTN_NewGame::FlashingOver);
	BindToAnimationFinished(flashing,Flashing);
	
}

void UBTN_NewGame::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void UBTN_NewGame::DelegatedAgentAndPlayAnimation()
{
	NewGame.Broadcast();
	PlayAnimation(flashing);
}

void UBTN_NewGame::FlashingOver()
{
	// if (ParentSwitcher)
	// {
	// 	ParentSwitcher->SetActiveWidgetIndex(1); // Set the index to the desired widget
	// }
	// ASW_HUD *HUD =Cast<ASW_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// ScriptManager = GetScriptManager();
	// ScriptManager->WidgetState = InGame;
	//
	//
	// HUD->Menu->InGameUI->ReadDialog();
}
