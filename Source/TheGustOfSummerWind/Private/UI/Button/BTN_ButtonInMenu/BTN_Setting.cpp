// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Setting.h"

void  UBTN_Setting::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Setting->OnClicked.AddDynamic(this,&UBTN_Setting::DelegatedAgent);
}

void  UBTN_Setting::DelegatedAgent()
{
	Setting.Broadcast();
}
