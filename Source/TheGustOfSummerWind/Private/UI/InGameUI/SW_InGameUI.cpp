// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_InGameUI.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/AudioComponent.h"
#include "Game/SW_HUD.h"


void USW_InGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeGame();
}

void USW_InGameUI::InitializeGame()
{
	ScriptManager = GetScriptManager();
	if (BP_NextDialog)
	{
		BP_NextDialog->EntrustDelegated.AddDynamic(this, &USW_InGameUI::PressBTN_NEXT);
	}
}

void USW_InGameUI::PressBTN_NEXT(int32 InRow)
{
	ReadDialog();
}

void USW_InGameUI::ReadDialog()
{
	SwitchChapter();
	ScriptManager = GetScriptManager();
	if (!ScriptManager)
	{
		return;
	}

	UpdateStruct();
	if (!DialogStruct || !PreviousDialogStruct || !BP_DialogBox)
	{
		return;
	}

	BP_DialogBox->SetDialogText(DialogStruct->Dialog);
	SetCharacter.Broadcast(*DialogStruct, *PreviousDialogStruct);

	SetName(DialogStruct);
	SetBackground(DialogStruct);
	SetMusic(DialogStruct);
	SetConversationalVoice(DialogStruct);
	DialogueRecord.Broadcast(DialogStruct->Dialog, DialogStruct->Name);

	CurrentIndex = 0;
	ScriptManager->rowDialog++;
}

void USW_InGameUI::UpdateStruct()
{
	if (!ScriptManager)
	{
		DialogStruct = nullptr;
		PreviousDialogStruct = nullptr;
		return;
	}

	DialogStruct = ScriptManager->GetDialogStruct();
	PreviousDialogStruct = ScriptManager->GetPreviousDialogStruct();
	if (!PreviousDialogStruct)
	{
		PreviousDialogStruct = DialogStruct;
	}
}

void USW_InGameUI::SetName(FDialogStruct* dialogRow)
{
	if (!dialogRow)
	{
		return;
	}

	if (dialogRow->Name.IsEmpty())
	{
		NameBoxBackground->SetBrushColor(FColor(0.0f, 0.0f, 0.0f, 0.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
	else
	{
		NameBoxBackground->SetBrushColor(FColor(255.0f, 255.0f, 255.0f, 255.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(255.0f, 255.0f, 255.0f, 255.0f));
		TextBlock_Name->SetText(dialogRow->Name);
	}
}

void USW_InGameUI::SetBackground(FDialogStruct* dialogRow)
{
	if (dialogRow)
	{
		TEX_Background->SetBrushFromTexture(dialogRow->Background, false);
	}
}

void USW_InGameUI::SetMusic(FDialogStruct* dialogRow)
{
	if (dialogRow && dialogRow->BackgroundSound != nullptr)
	{
		ScriptManager->BackgroundMusic = dialogRow->BackgroundSound;
		ScriptManager->AudioPlayer->Stop();
		ScriptManager->AudioPlayer->SetSound(dialogRow->BackgroundSound);
		ScriptManager->AudioPlayer->Play();
	}
}

void USW_InGameUI::SetConversationalVoice(FDialogStruct* dialogRow)
{
	if (dialogRow && dialogRow->ConversationalVoice != nullptr)
	{
		if (ScriptManager->ConversationalVoicePlayer != nullptr)
		{
			ScriptManager->ConversationalVoicePlayer->Stop();
		}
		ScriptManager->ConversationalVoicePlayer->SetSound(dialogRow->ConversationalVoice);
		ScriptManager->ConversationalVoicePlayer->Play();
	}
}

void USW_InGameUI::SwitchChapter()
{
	if (!ScriptManager)
	{
		return;
	}

	ScriptManager->CurrentChapter = ScriptManager->GetDataTableIndex(ScriptManager->DataTable);
	if (ScriptManager->rowDialog == ScriptManager->GetMaxDialogIndex() && ScriptManager->CurrentChapter >= 0)
	{
		UDataTable* NextChapter = ScriptManager->GetDataTableByIndex(ScriptManager->CurrentChapter + 1);
		if (NextChapter)
		{
			ScriptManager->rowDialog = 0;
			ScriptManager->CurrentChapter++;
			SwitchChapterCG.Broadcast();
			ScriptManager->DataTable = NextChapter;
		}
	}
}
