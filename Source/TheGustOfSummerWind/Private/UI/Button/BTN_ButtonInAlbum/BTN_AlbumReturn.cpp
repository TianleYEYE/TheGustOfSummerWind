// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInAlbum/BTN_AlbumReturn.h"

void UBTN_AlbumReturn::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void UBTN_AlbumReturn::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_AlbumReturn->OnClicked.AddDynamic(this,&UBTN_AlbumReturn::DelegatedAgent);
}

void UBTN_AlbumReturn::DelegatedAgent()
{
	AlbumReturn.Broadcast();
	
}
