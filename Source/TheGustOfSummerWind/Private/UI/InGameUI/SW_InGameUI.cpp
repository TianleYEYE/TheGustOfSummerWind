// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_InGameUI.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Components/AudioComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"


void USW_InGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	//初始化
	InitializeGame();
}

void USW_InGameUI::InitializeGame()
{
	//获取游戏实例，方便调用游戏实例中的全局函数
	SW_GameInstance=Cast<USW_GameInstance>(GetGameInstance());
	//获取场景中的ScriptManager
	auto TempActor = UGameplayStatics::GetActorOfClass(GetWorld(),ScriptManager->StaticClass());
	ScriptManager= Cast<ASW_ScriptManager>(TempActor);
	//下一段对话按钮绑定函数
	BP_NextDialog->EntrustDelegated.AddDynamic(this,&USW_InGameUI::PressBTN_NEXT);
	//绑定菜单开始动画时间，动画开始时，执行
	InGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::USW_InGameUI::GetInGameMenuUI);
	BindToAnimationStarted(DisplaysInGameMenu,InGameMenuUIEvent);
	//绑定菜单开始动画时间，动画结束后，执行
	CancelInGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::CancelInGameMenuUIImplement);
	BindToAnimationFinished(CancelDisplaysInGameMenu,CancelInGameMenuUIEvent);
	//获取对话行数
	row=SW_GameInstance->rowDialog;
	ReadDialog();
}

void USW_InGameUI::PressBTN_NEXT(int32 InRow)
{
	ReadDialog();
}

void USW_InGameUI::ReadDialog()
{
	//设置游戏实例的DialogStruct
	SW_GameInstance->SetDialogStruct(ScriptManager->DataTable);
	//从游戏实例获取到DialogStruct
	DialogStruct=SW_GameInstance->GetDialogStruct();
	//将UI获取的DilaogStruct给DialogBox
	BP_DialogBox->SetDialogStructAndCurrentIndex(DialogStruct,CurrentIndex);
	
	if (SW_GameInstance->rowDialog>=1)
	{
		PreviousDialogRow =SW_GameInstance->SetDialogStruct(DialogDataTable);
	}
	
	BP_DialogBox->SetDialog();
	SetName(DialogStruct);
	SetCharacterPortraits(DialogStruct);
	SetBackground(DialogStruct);
	SetMusic(DialogStruct);
	SetConversationalVoice(DialogStruct);
	CurrentIndex=0;
	SW_GameInstance->SetDialogIndex(row);
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Cyan,FString::Printf(TEXT("RowDialog : %d"),SW_GameInstance->rowDialog));
	row++;
	
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

void USW_InGameUI::SetCharacterPortraits(FDialogStruct *dialogRow)
{
	TEX_CharacterPortraits_1->SetBrushFromTexture(dialogRow->CharacterPortraits_1,true);
	TEX_CharacterPortraits_2->SetBrushFromTexture(dialogRow->CharacterPortraits_2,true);
	TEX_CharacterPortraits_3->SetBrushFromTexture(dialogRow->CharacterPortraits_3,true);


	if (PreviousDialogRow!=nullptr)
	{
			if (PreviousDialogRow->CharacterPortraits_1==nullptr && PreviousDialogRow->CharacterPortraits_2==nullptr && PreviousDialogRow->CharacterPortraits_2==nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2==nullptr && dialogRow->CharacterPortraits_3==nullptr))
		{
			PlayAnimation(One);
		}
		else if (PreviousDialogRow->CharacterPortraits_1!=nullptr && PreviousDialogRow->CharacterPortraits_2==nullptr && PreviousDialogRow->CharacterPortraits_3==nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2!=nullptr && dialogRow->CharacterPortraits_3==nullptr))
		{
			PlayAnimation(Two);
		}
		else if (PreviousDialogRow->CharacterPortraits_1!=nullptr && PreviousDialogRow->CharacterPortraits_2==nullptr && PreviousDialogRow->CharacterPortraits_3==nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2!=nullptr && dialogRow->CharacterPortraits_3!=nullptr))
		{
			PlayAnimation(Three);
		}
		else if (PreviousDialogRow->CharacterPortraits_1!=nullptr && PreviousDialogRow->CharacterPortraits_2!=nullptr && PreviousDialogRow->CharacterPortraits_3==nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2!=nullptr && dialogRow->CharacterPortraits_3!=nullptr))
		{
			PlayAnimation(Three);
		}
		else if (PreviousDialogRow->CharacterPortraits_1!=nullptr && PreviousDialogRow->CharacterPortraits_2!=nullptr && PreviousDialogRow->CharacterPortraits_3!=nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2!=nullptr && dialogRow->CharacterPortraits_3==nullptr))
		{
			PlayAnimationReverse(Three);
		}
		else if (PreviousDialogRow->CharacterPortraits_1!=nullptr && PreviousDialogRow->CharacterPortraits_2!=nullptr && PreviousDialogRow->CharacterPortraits_3!=nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2==nullptr && dialogRow->CharacterPortraits_3==nullptr))
		{
			PlayAnimationReverse(Three);
		}
		else if (PreviousDialogRow->CharacterPortraits_1!=nullptr && PreviousDialogRow->CharacterPortraits_2!=nullptr && PreviousDialogRow->CharacterPortraits_3==nullptr &&
			(dialogRow->CharacterPortraits_1!=nullptr && dialogRow->CharacterPortraits_2==nullptr && dialogRow->CharacterPortraits_3==nullptr))
		{
			PlayAnimationReverse(Two);
		}
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

void USW_InGameUI::GetCGOrStart()
{
	ScreenWhiteSlot->SetZOrder(1);
	
	bIsWhiteOver =true;
}

void USW_InGameUI::GetInGameMenuUI()
{
	inGameMenuUISlot=Cast<UCanvasPanelSlot>(InGameMenuUI->Slot);
	inGameMenuUISlot->SetZOrder(9);
}

void USW_InGameUI::CancelInGameMenuUIImplement()
{
	inGameMenuUISlot=Cast<UCanvasPanelSlot>(InGameMenuUI->Slot);
	inGameMenuUISlot->SetZOrder(1);
}