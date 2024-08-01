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
	Delegated();
	ReadDialog();
	
	BindToAnimationFinished(CancelDisplaysInGameMenu,CancelInGameMenuUIEvent);
}

void USW_InGameUI::Delegated()
{
	//下一段对话按钮绑定函数
	BP_NextDialog->EntrustDelegated.AddUObject(this,&USW_InGameUI::PressBTN_NEXT);
	CancelInGameMenuUIEvent.BindDynamic(this,&USW_InGameUI::CancelInGameMenuUIImplement);
}

void USW_InGameUI::PressBTN_NEXT(uint32& InRow)
{
	ReadDialog();
	InRow = rowDialog;
}

void USW_InGameUI::ReadDialog()
{
	GetDialogStruct(rowDialog);
	
	if (rowDialog>=1)
	{
		PreviousDialogRow =dialogDataTable->FindRow<FDialogStruct>(dialogDataTable->GetRowNames()[rowDialog-1],TEXT("1"));
	}
	
	SetName(DialogRow);
	
	SetDialog();
	
	SetCharacterPortraits(DialogRow);

	SetBackground(DialogRow);
	
	SetMusic(DialogRow);
	
	SetConversationalVoice(DialogRow);
	
	rowDialog++;

	CurrentIndex=0;
}


void USW_InGameUI::SetDialog()
{
	GetWorld()->GetTimerManager().SetTimer(BP_DialogBox->UpdataTextHandle, this, &USW_InGameUI::UpdateText, 0.05f, true);
}
void USW_InGameUI::UpdateText()
{
	
	MessageCharactArray=UKismetStringLibrary::GetCharacterArrayFromString(DialogRow->Dialog.ToString());
	
	if (CurrentIndex < MessageCharactArray.Num())
    {
        // 获取当前要显示的文本
        FText CurrentText = FText::FromString(DialogRow->Dialog.ToString().Left(CurrentIndex + 1));
        
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

// void USW_InGameUI::SetGameCG(FDialogStruct* dialogRow,int row)
// {
// 	GameCGSlot=Cast<UCanvasPanelSlot>(GameCG->Slot);
// 	ScreenWhiteSlot=Cast<UCanvasPanelSlot>(ScreenWhite->Slot);
// 	//当游戏开始时播放一次白屏渐变
// 	if (row==0 )
// 	{
// 		ScreenWhiteSlot->SetZOrder(10);
// 		
// 		PlayAnimation(ScreenToWhite);
// 		bIsWhiteOver =false;
// 	}
// 	//当游戏有cg有CG且上段对话没有cg时播放一次白屏渐变
// 	else if (dialogRow->GameCG!=nullptr && PreviousDialogRow->GameCG ==nullptr)
// 	{
// 		ScreenWhiteSlot->SetZOrder(10);
// 		
// 		GameCGSlot->SetZOrder(3);
// 		
// 		PlayAnimation(ScreenToWhite);
// 		bIsWhiteOver =false;
// 	}
// 	//当游戏有cg且上段对话也有cg时
// 	else if (dialogRow->GameCG!=nullptr && PreviousDialogRow->GameCG !=nullptr)
// 	{
// 		GameCGSlot->SetZOrder(3);
// 	}
// 	//当游戏没有CG时将CG放置在游戏底部以免遮挡
// 	else if ( dialogRow->GameCG==nullptr)
// 	{
// 		GameCGSlot->SetZOrder(1);
// 	}
// 	GameCG->SetBrushFromTexture(dialogRow->GameCG);
// }

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

	PlayAnimation(DisplaysInGameMenu);
}

void USW_InGameUI::CancelInGameMenuUI()
{
	PlayAnimation(CancelDisplaysInGameMenu);
}

void USW_InGameUI::CancelInGameMenuUIImplement()
{
	inGameMenuUISlot=Cast<UCanvasPanelSlot>(InGameMenuUI->Slot);
	inGameMenuUISlot->SetZOrder(1);
}