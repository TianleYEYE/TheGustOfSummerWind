// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInAlbum/BTN_Previous.h"
void UBTN_Previous::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Previous->OnClicked.AddDynamic(this,&UBTN_Previous::DelegatedAgent);
}

void UBTN_Previous::DelegatedAgent()
{
	Previous.Broadcast();
}

