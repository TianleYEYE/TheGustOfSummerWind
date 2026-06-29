// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InMenuUI/SW_InMenuUI.h"

#include "Components/CanvasPanelSlot.h"
#include "Game/SW_ScriptManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void USW_InMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (FullScreenBlack)
	{
		TurnBlack.BindDynamic(this, &USW_InMenuUI::FlashingOver);
		BindToAnimationFinished(FullScreenBlack, TurnBlack);
	}

	if (Fade)
	{
		PlayAnimation(Fade);
	}

	// Bind button delegates
	if (BP_ButtonOfInMenu)
	{
		if (BP_ButtonOfInMenu->BP_NewGame)
		{
			BP_ButtonOfInMenu->BP_NewGame->NewGame.AddUObject(this, &USW_InMenuUI::PressBTN_NewGame);
		}
		if (BP_ButtonOfInMenu->BP_Album)
		{
			BP_ButtonOfInMenu->BP_Album->Album.AddUObject(this, &USW_InMenuUI::PressBTN_Album);
		}
		if (BP_ButtonOfInMenu->BP_Continue)
		{
			BP_ButtonOfInMenu->BP_Continue->Continue.AddDynamic(this, &USW_InMenuUI::PressBTN_Continue);
		}
		if (BP_ButtonOfInMenu->BP_Setting)
		{
			BP_ButtonOfInMenu->BP_Setting->Setting.AddUObject(this, &USW_InMenuUI::PressBTN_Setting);
		}
	}
}

void USW_InMenuUI::PressBTN_NewGame()
{
	PlayATapSound();

	CurrentTransition = ETransitionTarget::NewGame;
	SetScreenBlackZOrder(3);
	if (FullScreenBlack)
	{
		PlayAnimation(FullScreenBlack);
	}
	else
	{
		OnFullScreenBlackFinished();
	}
}

void USW_InMenuUI::PressBTN_Album()
{
	PlayATapSound();

	CurrentTransition = ETransitionTarget::Album;
	SetScreenBlackZOrder(3);
	if (FullScreenBlack)
	{
		PlayAnimation(FullScreenBlack);
	}
	else
	{
		OnFullScreenBlackFinished();
	}
}

void USW_InMenuUI::PressBTN_Continue()
{
	PlayATapSound();

	CurrentTransition = ETransitionTarget::Continue;
	SetScreenBlackZOrder(3);
	if (FullScreenBlack)
	{
		PlayAnimation(FullScreenBlack);
	}
	else
	{
		OnFullScreenBlackFinished();
	}
}

void USW_InMenuUI::PressBTN_Setting()
{
	PlayATapSound();

	CurrentTransition = ETransitionTarget::Setting;
	SetScreenBlackZOrder(3);
	if (FullScreenBlack)
	{
		PlayAnimation(FullScreenBlack);
	}
	else
	{
		OnFullScreenBlackFinished();
	}
}

void USW_InMenuUI::FlashingOver()
{
	if (ScreenBlack)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ScreenBlack->Slot))
		{
			CanvasSlot->SetZOrder(0);
		}
	}

	OnFullScreenBlackFinished();
}

void USW_InMenuUI::OnFullScreenBlackFinished()
{
	switch (CurrentTransition)
	{
	case ETransitionTarget::NewGame:
		OnNewGameTransition.Broadcast();
		break;
	case ETransitionTarget::Continue:
		OnContinueTransition.Broadcast();
		break;
	case ETransitionTarget::Album:
		OnAlbumTransition.Broadcast();
		break;
	case ETransitionTarget::Setting:
		OnSettingTransition.Broadcast();
		break;
	default:
		break;
	}

	CurrentTransition = ETransitionTarget::None;
}

void USW_InMenuUI::PlayATapSound()
{
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}
}

void USW_InMenuUI::SetScreenBlackZOrder(int32 NewZOrder)
{
	if (ScreenBlack)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ScreenBlack->Slot))
		{
			CanvasSlot->SetZOrder(NewZOrder);
		}
	}
}
