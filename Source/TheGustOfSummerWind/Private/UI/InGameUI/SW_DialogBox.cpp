// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_DialogBox.h"

void USW_DialogBox::NativeConstruct()
{
	Super::NativeConstruct();
}

void USW_DialogBox::SetDialogText(FText& Text)
{
	UpdateDialogText.Broadcast(Text);
}
