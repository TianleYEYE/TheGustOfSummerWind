// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_InGameUI.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/AudioComponent.h"
#include "Game/SW_HUD.h"



void USW_InGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	//初始化
	InitializeGame();
}


void USW_InGameUI::InitializeGame()
{
	//获取场景中的ScriptManager
	ScriptManager = GetScriptManager();
	//下一段对话按钮绑定函数
	BP_NextDialog->EntrustDelegated.AddDynamic(this,&USW_InGameUI::PressBTN_NEXT);
}

void USW_InGameUI::PressBTN_NEXT(int32 InRow)
{
	ReadDialog();
	
}

void USW_InGameUI::ReadDialog()
{
	SwitchChapter();
	ScriptManager = GetScriptManager();
	UpdateStruct();
	
	BP_DialogBox->SetDialogText(DialogStruct->Dialog);

	SetCharacter.Broadcast(*DialogStruct,*PreviousDialogStruct);
	
	SetName(DialogStruct);
	SetBackground(DialogStruct);
	SetMusic(DialogStruct);
	SetConversationalVoice(DialogStruct);

	
	CurrentIndex=0;
	ScriptManager->rowDialog++;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,FString::Printf(TEXT("Dialog : %d"),	ScriptManager->rowDialog));
}

void USW_InGameUI::UpdateStruct()
{
	DialogStruct = ScriptManager->GetDialogStruct();
	if(ScriptManager->GetPreviousDialogStruct())
	{
		PreviousDialogStruct = ScriptManager->GetPreviousDialogStruct();
	}
	else
	{
		PreviousDialogStruct =new FDialogStruct();
	}
}

void USW_InGameUI::SetName(FDialogStruct *dialogRow)
{
	if (dialogRow->Name.IsEmpty())
	{
		NameBoxBackground->SetBrushColor(FColor(0.0f,0.0f,0.0f,0.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(0.0f,0.0f,0.0f,0.0f));
	}
	else
	{
		NameBoxBackground->SetBrushColor(FColor(255.0f,255.0f,255.0f,255.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(255.0f,255.0f,255.0f,255.0f));
		TextBlock_Name->SetText(dialogRow->Name);
	}
}

void USW_InGameUI::SetBackground(FDialogStruct *dialogRow)
{
	TEX_Background->SetBrushFromTexture(dialogRow->Background,false);

}

void USW_InGameUI::SetMusic(FDialogStruct *dialogRow)
{
	if (dialogRow->BackgroundSound!=nullptr)
	{
		ScriptManager->BackgroundMusic = dialogRow->BackgroundSound;
		//SetSoundMixClassOver
		ScriptManager->AudioPlayer->Stop();
		ScriptManager->AudioPlayer->SetSound(dialogRow->BackgroundSound);
		ScriptManager->AudioPlayer->Play();
	}
	
}

void USW_InGameUI::SetConversationalVoice(FDialogStruct *dialogRow)
{
	if (dialogRow->ConversationalVoice!=nullptr)
	{
		if (ScriptManager->ConversationalVoicePlayer!=nullptr)
		{
			ScriptManager->ConversationalVoicePlayer->Stop();
		}
		ScriptManager->ConversationalVoicePlayer->SetSound(dialogRow->ConversationalVoice);
		ScriptManager->ConversationalVoicePlayer->Play();
	}
}

void USW_InGameUI::SwitchChapter()
{
	if (ScriptManager->rowDialog == ScriptManager->GetMaxDialogIndex() && ScriptManager->CurrentChapter < 15)
	{
		ScriptManager->rowDialog = 0;
		ScriptManager->CurrentChapter++;
		PlayAnimation(Fade);
		UDataTable * CurrentChapter = ScriptManager->GetDataTableByIndex(ScriptManager->CurrentChapter);
		ScriptManager->DataTable = CurrentChapter;
	}

	ScriptManager->CurrentChapter = ScriptManager->GetDataTableIndex(ScriptManager->DataTable);
}



