// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Album.h"

void UBTN_Album::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Album->OnClicked.AddDynamic(this,&UBTN_Album::DelegatedAgentAndPlayAnimation);
	Flashing.BindDynamic(this,&UBTN_Album::FlashingOver);
	BindToAnimationFinished(flashing,Flashing);
}

void UBTN_Album::DelegatedAgentAndPlayAnimation()
{
	Album.Broadcast();
	PlayAnimation(flashing);
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
