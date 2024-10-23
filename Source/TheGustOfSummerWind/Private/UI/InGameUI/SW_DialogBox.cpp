// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_DialogBox.h"
#include "Components/Throbber.h"
#include "Kismet/KismetStringLibrary.h"

void USW_DialogBox::NativeConstruct()
{
	Super::NativeConstruct();

	if (Throbber_1)
	{
		Throbber_1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USW_DialogBox::SetDialogStructAndCurrentIndex(FDialogStruct * ParentsDialogStruct,int Index)
{
	DialogRow=ParentsDialogStruct;
	CurrentIndex=Index;
}

void USW_DialogBox::SetDialog()
{
	Throbber_1->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(UpdataTextHandle, this, &USW_DialogBox::UpdateText, 0.1f, true);
}

void USW_DialogBox::UpdateText()
{
	MessageCharactArray=UKismetStringLibrary::GetCharacterArrayFromString(DialogRow->Dialog.ToString());
	
	if (CurrentIndex < MessageCharactArray.Num())
	{
		// 获取当前要显示的文本
		FText CurrentText = FText::FromString(DialogRow->Dialog.ToString().Left(CurrentIndex + 1));
        
		// 设置 TextBlock 的文本
		if (Textblock_Dialog)
		{
			Textblock_Dialog->SetText(CurrentText);
			
		}
        
		// 增加索引
		CurrentIndex++;
	}
	else
	{
		// 停止定时器
		GetWorld()->GetTimerManager().ClearTimer(UpdataTextHandle);
		Throbber_1->SetVisibility(ESlateVisibility::Hidden);
	}
}
