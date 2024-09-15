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

void USW_DialogBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateVisibility();
}


void USW_DialogBox::UpdateVisibility()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(UpdataTextHandle))
	{
		Throbber_1->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Throbber_1->SetVisibility(ESlateVisibility::Hidden);
	}

	// 确保持续更新
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]
		{
			UpdateVisibility();
		});
	}
}

void USW_DialogBox::SetDialogStructAndCurrentIndex(FDialogStruct * ParentsDialogStruct,int Index)
{
	DialogRow=ParentsDialogStruct;
	CurrentIndex=Index;
}

void USW_DialogBox::SetDialog()
{
	GetWorld()->GetTimerManager().SetTimer(UpdataTextHandle, this, &USW_DialogBox::UpdateText, 0.05f, true);
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
	}
}
