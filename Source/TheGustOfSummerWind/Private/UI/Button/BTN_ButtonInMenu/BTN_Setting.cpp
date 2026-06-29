// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Setting.h"

void UBTN_Setting::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void  UBTN_Setting::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_Setting)
	{
		BTN_Setting->OnClicked.AddUniqueDynamic(this, &UBTN_Setting::DelegatedAgentAndPlayAnimation);
	}
	if (flashing)
	{
		Flashing.BindDynamic(this, &UBTN_Setting::FlashingOver);
		BindToAnimationFinished(flashing, Flashing);
	}
}

void  UBTN_Setting::DelegatedAgentAndPlayAnimation()
{
	Setting.Broadcast();
	if (flashing)
	{
		PlayAnimation(flashing);
	}
}

void UBTN_Setting::FlashingOver()
{
}
