// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_CharacterPortraits.h"


void USW_CharacterPortraits::NativeConstruct()

{
	Super::NativeConstruct();

	
	//BindToAnimationFinished(FaceChange,FaceChangeEvent);
	
}

void USW_CharacterPortraits::SetCharacterPortraits()
{
	
	ScriptManager = GetScriptManager();
	DialogStruct = ScriptManager->GetDialogStruct();
	if(ScriptManager->GetPreviousDialogStruct())
	{
		PreviousDialogStruct = ScriptManager->GetPreviousDialogStruct();
	}
	else
	{
		PreviousDialogStruct =new FDialogStruct();
	}
	

	SetCharacter.Broadcast(*DialogStruct,*PreviousDialogStruct);
	
}

 
