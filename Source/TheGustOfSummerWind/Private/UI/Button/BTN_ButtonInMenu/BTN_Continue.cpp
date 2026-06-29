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
	if (flashing)
	{
		Flashing.BindDynamic(this, &UBTN_Continue::FlashingOver);
		BindToAnimationFinished(flashing, Flashing);
	}
}

void UBTN_Continue::DelegatedAgentAndPlayAnimation()
{
	Continue.Broadcast();
	if (flashing)
	{
		PlayAnimation(flashing);
	}
}

void UBTN_Continue::FlashingOver()
{
	
}
