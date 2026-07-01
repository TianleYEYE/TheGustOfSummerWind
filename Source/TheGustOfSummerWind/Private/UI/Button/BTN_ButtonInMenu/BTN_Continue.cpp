// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Continue.h"

#include "Components/WidgetSwitcher.h"

void UBTN_Continue::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_Continue)
	{
		BTN_Continue->OnClicked.AddUniqueDynamic(this, &UBTN_Continue::DelegatedAgentAndPlayAnimation);
	}
}

void UBTN_Continue::DelegatedAgentAndPlayAnimation()
{
	PlayPrimaryActionAnimationOrBroadcast();
}

void UBTN_Continue::BroadcastPrimaryAction()
{
	Continue.Broadcast();
}

void UBTN_Continue::FlashingOver()
{
	
}
