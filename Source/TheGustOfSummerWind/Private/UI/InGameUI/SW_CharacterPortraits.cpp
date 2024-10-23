// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_CharacterPortraits.h"


void USW_CharacterPortraits::NativeConstruct()
{
	Super::NativeConstruct();

	FaceChangeEvent.BindDynamic(this,&USW_CharacterPortraits::DoFaceChange);
	BindToAnimationFinished(FaceChange,FaceChangeEvent);
}

void USW_CharacterPortraits::SetCharacterPortraits()
{
	//判断动画是否还在
	if (IsAnimationPlaying(FaceChange) && GetAnimationCurrentTime(FaceChange) <= 1.f)
	{
		SetAnimationCurrentTime(FaceChange,0.f);
		PauseAnimation(FaceChange);
	}

	
	ScriptManager = GetScriptManager();
	DialogStruct = ScriptManager->SetDialogStruct();
	VerticalPainting->SetBrushFromTexture(DialogStruct->CharacterPortraits_1,true);
	Face_1->SetBrushFromTexture(DialogStruct->Face_1,true);
	Face_2->SetBrushFromTexture(DialogStruct->Face_2,true);

	Face_1->SetColorAndOpacity(FColor(255.f,255.f,255.f));

	if (DialogStruct->Face_2 != nullptr)
	{
		PlayAnimation(FaceChange);
	}
}

void USW_CharacterPortraits::DoFaceChange()
{
	
}


