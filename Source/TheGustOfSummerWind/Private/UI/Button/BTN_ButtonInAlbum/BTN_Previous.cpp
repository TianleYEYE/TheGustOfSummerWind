// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInAlbum/BTN_Previous.h"
void UBTN_Previous::NativeConstruct()
{
	Super::NativeConstruct();
	if (BTN_Previous)
	{
		BTN_Previous->OnClicked.AddUniqueDynamic(this, &UBTN_Previous::DelegatedAgent);
	}
}

void UBTN_Previous::DelegatedAgent()
{
	Previous.Broadcast();
}
