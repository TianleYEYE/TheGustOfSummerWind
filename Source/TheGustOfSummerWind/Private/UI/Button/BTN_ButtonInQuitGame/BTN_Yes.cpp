// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_ButtonInQuitGame/BTN_Yes.h"

void UBTN_Yes::NativeConstruct()
{
	Super::NativeConstruct();
	BTN_Yes->OnClicked.AddDynamic(this,&UBTN_Yes::DelegatedAgent);
}

void UBTN_Yes::DelegatedAgent()
{
	Yes.Broadcast();
}

