// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Continue.h"

void UBTN_Continue::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Continue->OnClicked.AddDynamic(this,&UBTN_Continue::DelegatedAgent);
}

void UBTN_Continue::DelegatedAgent()
{
	Continue.Broadcast();
}
