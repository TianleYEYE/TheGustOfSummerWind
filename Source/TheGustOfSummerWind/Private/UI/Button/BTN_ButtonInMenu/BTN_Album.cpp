// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Album.h"

void UBTN_Album::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_Album)
	{
		BTN_Album->OnClicked.AddUniqueDynamic(this, &UBTN_Album::DelegatedAgentAndPlayAnimation);
	}
}

void UBTN_Album::DelegatedAgentAndPlayAnimation()
{
	PlayPrimaryActionAnimationOrBroadcast();
}

void UBTN_Album::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void UBTN_Album::FlashingOver()
{
	// if (ParentSwitcher)
	// {
	// 	ParentSwitcher->SetActiveWidgetIndex(3); // Set the index to the desired widget
	// }
	// ScriptManager = GetScriptManager();
	// ScriptManager ->WidgetState = AlbumUI;
}

void UBTN_Album::BroadcastPrimaryAction()
{
	Album.Broadcast();
}
