// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_InGameUI.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Components/AudioComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Music/AudioPlayer.h"



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
	//下一段对话按钮绑定函数
	BP_NextDialog->EntrustDelegated.AddUObject(this,&USW_InGameUI::PressBTN_NEXT);

	//绑定菜单开始动画时间，动画开始时，执行
	InGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::USW_InGameUI::GetInGameMenuUI);
	BindToAnimationStarted(DisplaysInGameMenu,InGameMenuUIEvent);
	//绑定菜单开始动画时间，动画结束后，执行
	CancelInGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::CancelInGameMenuUIImplement);
	BindToAnimationFinished(CancelDisplaysInGameMenu,CancelInGameMenuUIEvent);
	
	//获取对话行数
	row=SW_GameInstance->GlobalVariablesManger->rowDialog;
	
	//开始游戏时读取一次对话
	ReadDialog();
}

void USW_InGameUI::PressBTN_NEXT(uint32& InRow)
{
	ReadDialog();
	InRow = row;
}

void USW_InGameUI::ReadDialog()
{
	
	DialogStruct=SW_GameInstance->GlobalVariablesManger->GetDialogStruct(DialogDataTable,row);
	
	if (SW_GameInstance->GlobalVariablesManger->rowDialog>=1)
	{
		PreviousDialogRow =SW_GameInstance->GlobalVariablesManger->GetDialogStruct(DialogDataTable,row-1);
	}
	
	SetName(DialogStruct);
	SetDialog();
	SetCharacterPortraits(DialogStruct);
	SetBackground(DialogStruct);
	SetMusic(DialogStruct);
	SetConversationalVoice(DialogStruct);
	
	row++;
	CurrentIndex=0;
}

void USW_InGameUI::SetDialog()
{
	GetWorld()->GetTimerManager().SetTimer(BP_DialogBox->UpdataTextHandle, this, &USW_InGameUI::UpdateText, 0.05f, true);
}

void USW_InGameUI::UpdateText()
{
	
	MessageCharactArray=UKismetStringLibrary::GetCharacterArrayFromString(DialogStruct->Dialog.ToString());
	
	if (CurrentIndex < MessageCharactArray.Num())
    {
        // 获取当前要显示的文本
        FText CurrentText = FText::FromString(DialogStruct->Dialog.ToString().Left(CurrentIndex + 1));
        
        // 设置 TextBlock 的文本
        if (BP_DialogBox->Textblock_Dialog)
        {
           BP_DialogBox->Textblock_Dialog->SetText(CurrentText);
        }
        
        // 增加索引
        CurrentIndex++;
    }
    else
    {
    	  
        // 停止定时器
        GetWorld()->GetTimerManager().ClearTimer(BP_DialogBox->UpdataTextHandle);
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
	auto TempActor = UGameplayStatics::GetActorOfClass(GetWorld(),AudioPlayer->StaticClass());
	AudioPlayer = Cast<AAudioPlayer>(TempActor);
	if (dialogRow->BackgroundSound!=nullptr)
	{
		AudioPlayer->audioPlayer->Stop();
		AudioPlayer->audioPlayer->SetSound(dialogRow->BackgroundSound);
		AudioPlayer->audioPlayer->Play();
	}
}

void USW_InGameUI::SetConversationalVoice(FDialogStruct *dialogRow)
{
	auto TempActor = UGameplayStatics::GetActorOfClass(GetWorld(),AudioPlayer->StaticClass());
	AudioPlayer = Cast<AAudioPlayer>(TempActor);
	if (dialogRow->ConversationalVoice!=nullptr)
	{
		if (AudioPlayer->ConversationalVoicePlayer!=nullptr)
		{
			AudioPlayer->ConversationalVoicePlayer->Stop();
		}
		AudioPlayer->ConversationalVoicePlayer->SetSound(dialogRow->ConversationalVoice);
		AudioPlayer->ConversationalVoicePlayer->Play();
	}
}

void USW_InGameUI::GetCGOrStart()
{
	ScreenWhiteSlot->SetZOrder(1);

	SetDialog();
	
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