// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInMenu/BTN_Album.h"

void UBTN_Album::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Album->OnClicked.AddDynamic(this,&UBTN_Album::DelegatedAgent);
}

void UBTN_Album::DelegatedAgent()
{
	Album.Broadcast();
}
