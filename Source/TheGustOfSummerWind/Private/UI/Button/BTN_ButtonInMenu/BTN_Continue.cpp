// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Continue.h"

#include "Components/WidgetSwitcher.h"

void UBTN_Continue::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Continue->OnClicked.AddDynamic(this,&UBTN_Continue::DelegatedAgentAndPlayAnimation);
	Flashing.BindDynamic(this,&UBTN_Continue::UBTN_Continue::FlashingOver);
	BindToAnimationFinished(flashing,Flashing);
}

void UBTN_Continue::DelegatedAgentAndPlayAnimation()
{
	Continue.Broadcast();
}

void UBTN_Continue::FlashingOver()
{
	
}


