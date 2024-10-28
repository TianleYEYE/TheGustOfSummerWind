// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName,InLoadSlotName);
}

void UMVVM_LoadSlot::SetBackgroundMusic(USoundBase* InBackgroundMusic)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackgroundMusic,InBackgroundMusic);
}

void UMVVM_LoadSlot::SetDataTable(UDataTable* InDataTable)
{
	UE_MVVM_SET_PROPERTY_VALUE(DataTable,InDataTable);
}

void UMVVM_LoadSlot::SetChapterName(FString InChapterName)
{
	if (InChapterName == FString("DT_Chapter_1"))
	{
		InChapterName = FString("第一章");
	}
	else if (InChapterName == FString("DT_Chapter_2"))
	{
		InChapterName = FString("第二章");
	}
	else if (InChapterName == FString("DT_Chapter_3"))
	{
		InChapterName = FString("第三章");
	}
	else if (InChapterName == FString("DT_Chapter_4"))
	{
		InChapterName = FString("第四章");
	}
	else if (InChapterName == FString("DT_Chapter_5"))
	{
		InChapterName = FString("第五章");
	}
	else if (InChapterName == FString("DT_Chapter_6"))
	{
		InChapterName = FString("第六章");
	}
	else if (InChapterName == FString("DT_Chapter_7"))
	{
		InChapterName = FString("第七章");
	}
	else if (InChapterName == FString("DT_Chapter_8"))
	{
		InChapterName = FString("第八章");
	}
	else if (InChapterName == FString("DT_Chapter_9"))
	{
		InChapterName = FString("第九章");
	}
	else if (InChapterName == FString("DT_Chapter_10"))
	{
		InChapterName = FString("第十章");
	}
	else if (InChapterName == FString("DT_Chapter_11"))
	{
		InChapterName = FString("第十一章");
	}
	else if (InChapterName == FString("DT_Chapter_12"))
	{
		InChapterName = FString("第十二章");
	}
	else if (InChapterName == FString("DT_Chapter_13"))
	{
		InChapterName = FString("第十三章");
	}
	else if (InChapterName == FString("DT_Chapter_14"))
	{
		InChapterName = FString("第十四章");
	}
	
	
	UE_MVVM_SET_PROPERTY_VALUE(ChapterName,InChapterName);
}

void UMVVM_LoadSlot::SetRowDialog(int32 InRowDialog)
{
	UE_MVVM_SET_PROPERTY_VALUE(RowDialog,InRowDialog);
}
