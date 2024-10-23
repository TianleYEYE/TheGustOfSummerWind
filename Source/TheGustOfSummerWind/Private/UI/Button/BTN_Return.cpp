// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_Return.h"

void UBTN_Return::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Return->OnClicked.AddDynamic(this,&UBTN_Return::DelegatedAgent);
}

void UBTN_Return::DelegatedAgent()
{
	Return.Broadcast();
}
