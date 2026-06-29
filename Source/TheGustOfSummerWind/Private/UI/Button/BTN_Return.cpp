// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BTN_Return.h"

void UBTN_Return::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Return)
	{
		BTN_Return->OnClicked.AddUniqueDynamic(this, &UBTN_Return::DelegatedAgent);
	}
}

void UBTN_Return::DelegatedAgent()
{
	Return.Broadcast();
}
