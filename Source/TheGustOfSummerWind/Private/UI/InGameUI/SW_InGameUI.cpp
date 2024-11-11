// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_InGameUI.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Components/AudioComponent.h"
#include "Components/CanvasPanelSlot.h"
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
	//绑定菜单开始动画时间，动画开始时，执行
	InGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::GetInGameMenuUI);
	BindToAnimationStarted(DisplaysInGameMenu,InGameMenuUIEvent);
	//绑定菜单开始动画时间，动画结束后，执行
	CancelInGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::CancelInGameMenuUIImplement);
	BindToAnimationFinished(CancelDisplaysInGameMenu,CancelInGameMenuUIEvent);
}

void USW_InGameUI::PressBTN_NEXT(int32 InRow)
{
	ReadDialog();
	
}

void USW_InGameUI::ReadDialog()
{
	SwitchChapter();
	// //从游戏脚本管理器获取到DialogStruct
	DialogStruct=ScriptManager->SetDialogStruct();
	
	if (ScriptManager->rowDialog>=1)
	{
		PreviousDialogRow =ScriptManager->PreviousDialogStruct;
	}
	
	BP_DialogBox->SetDialogText(DialogStruct->Dialog);
	BP_CharacterPortraits->SetCharacterPortraits();
	SetName(DialogStruct);
	SetBackground(DialogStruct);
	SetMusic(DialogStruct);
	SetConversationalVoice(DialogStruct);

	
	CurrentIndex=0;
	ScriptManager->rowDialog++;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,FString::Printf(TEXT("Dialog : %d"),	ScriptManager->rowDialog));
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
		NameBoxBackground->SetBrushColor(FColor(200.0f,235.0f,228.0f,255.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(255.0f,255.0f,255.0f,255.0f));
		TextBlock_Name->SetText(dialogRow->Name);
	}
}

void USW_InGameUI::SetBackground(FDialogStruct *dialogRow)
{
	TEX_Background->SetBrushFromTexture(dialogRow->Background,true);
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

void USW_InGameUI::GetInGameMenuUI()
{
	inGameMenuUISlot=Cast<UCanvasPanelSlot>(InGameMenuUI->Slot);
	inGameMenuUISlot->SetZOrder(9);
	bIsInGameMenuUIExist=true;
}

void USW_InGameUI::CancelInGameMenuUIImplement()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("CancelInGameMenuUIImplement"));
	GetScriptManager()->WidgetState = InGame;
	inGameMenuUISlot=Cast<UCanvasPanelSlot>(InGameMenuUI->Slot);
	inGameMenuUISlot->SetZOrder(0);
	bIsInGameMenuUIExist=false;
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



