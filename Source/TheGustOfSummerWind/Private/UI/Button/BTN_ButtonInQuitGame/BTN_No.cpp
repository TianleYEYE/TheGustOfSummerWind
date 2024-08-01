// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInQuitGame/BTN_No.h"

void UBTN_No::NativeConstruct()
{
	
	Super::NativeConstruct();
	BTN_No->OnClicked.AddDynamic(this,&UBTN_No::DelegatedAgent);
}

void UBTN_No::DelegatedAgent()
{
	No.Broadcast();
}
