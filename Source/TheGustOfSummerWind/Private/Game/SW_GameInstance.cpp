// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_GameInstance.h"

void USW_GameInstance::Init()
{
	Super::Init();

	GlobalVariablesManger=NewObject<UGlobalVariablesManager>(this);
}
