// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_NewGame.h"

void UBTN_NewGame::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_NewGame->OnClicked.AddDynamic(this,&UBTN_NewGame::DelegatedAgent);
	
}

void UBTN_NewGame::DelegatedAgent()
{
	NewGame.Broadcast();
	
}

