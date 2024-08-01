// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInAlbum/BTN_Next.h"

void  UBTN_Next::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Next->OnClicked.AddDynamic(this,&UBTN_Next::DelegatedAgent);
}

void UBTN_Next::DelegatedAgent()
{
	Next.Broadcast();
}
