// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SettingUI/SW_SettingUI.h"

void USW_SettingUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Fade)
	{
		FWidgetAnimationDynamicEvent FadeFinished;
		FadeFinished.BindDynamic(this, &USW_SettingUI::OnFadeFinished);
		BindToAnimationFinished(Fade, FadeFinished);
	}
}

void USW_SettingUI::PlayFadeOut()
{
	if (Fade)
	{
		PlayAnimation(Fade);
	}
}

void USW_SettingUI::PlayFadeIn()
{
	if (Fade)
	{
		PlayAnimation(Fade, 0.0f, 1, EUMGSequencePlayMode::Reverse);
	}
}

void USW_SettingUI::OnFadeFinished()
{
	OnSettingFadeComplete.Broadcast();
}
