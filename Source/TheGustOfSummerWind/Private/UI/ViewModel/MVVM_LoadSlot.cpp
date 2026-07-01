// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetBackgroundMusic(USoundBase* InBackgroundMusic)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackgroundMusic, InBackgroundMusic);
}

void UMVVM_LoadSlot::SetDataTable(UDataTable* InDataTable)
{
	UE_MVVM_SET_PROPERTY_VALUE(DataTable, InDataTable);
}

void UMVVM_LoadSlot::SetChapterName(FString InChapterName)
{
	if (InChapterName.StartsWith(TEXT("DT_Chapter_")))
	{
		const FString ChapterIndexString = InChapterName.RightChop(11);
		const int32 ChapterIndex = FCString::Atoi(*ChapterIndexString);
		if (ChapterIndex > 0)
		{
			InChapterName = FString::Printf(TEXT("第%d章"), ChapterIndex);
		}
	}

	UE_MVVM_SET_PROPERTY_VALUE(ChapterName, InChapterName);
}

void UMVVM_LoadSlot::SetRowDialog(int32 InRowDialog)
{
	UE_MVVM_SET_PROPERTY_VALUE(RowDialog, InRowDialog);
}

void UMVVM_LoadSlot::SetDateTime(FDateTime InDateTime)
{
	UE_MVVM_SET_PROPERTY_VALUE(DateTime, InDateTime);
}
