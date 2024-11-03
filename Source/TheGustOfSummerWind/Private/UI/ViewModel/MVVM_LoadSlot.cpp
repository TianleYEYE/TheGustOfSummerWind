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
	UE_LOG(LogTemp, Warning, TEXT("%hs -- %s"), __FUNCTION__, *InChapterName);
	if (InChapterName == TEXT("DT_Chapter_1"))
	{
		InChapterName = TEXT("第一章");
	}
	else if (InChapterName == TEXT("DT_Chapter_2"))
	{
		InChapterName = TEXT("第二章");
	}
	else if (InChapterName == TEXT("DT_Chapter_3"))
	{
		InChapterName = TEXT("第三章");
	}
	else if (InChapterName == TEXT("DT_Chapter_4"))
	{
		InChapterName = TEXT("第四章");
	}
	else if (InChapterName == TEXT("DT_Chapter_5"))
	{
		InChapterName = TEXT("第五章");
	}
	else if (InChapterName == TEXT("DT_Chapter_6"))
	{
		InChapterName = TEXT("第六章");
	}
	else if (InChapterName == TEXT("DT_Chapter_7"))
	{
		InChapterName = TEXT("第七章");
	}
	else if (InChapterName == TEXT("DT_Chapter_8"))
	{
		InChapterName = TEXT("第八章");
	}
	else if (InChapterName == TEXT("DT_Chapter_9"))
	{
		InChapterName = TEXT("第九章");
	}
	else if (InChapterName == TEXT("DT_Chapter_10"))
	{
		InChapterName = TEXT("第十章");
	}
	else if (InChapterName == TEXT("DT_Chapter_11"))
	{
		InChapterName = TEXT("第十一章");
	}
	else if (InChapterName == TEXT("DT_Chapter_12"))
	{
		InChapterName = TEXT("第十二章");
	}
	else if (InChapterName == TEXT("DT_Chapter_13"))
	{
		InChapterName = TEXT("第十三章");
	}
	else if (InChapterName == TEXT("DT_Chapter_14"))
	{
		InChapterName = TEXT("第十四章");
	}
	
	
	UE_MVVM_SET_PROPERTY_VALUE(ChapterName,InChapterName);
}

void UMVVM_LoadSlot::SetRowDialog(int32 InRowDialog)
{
	UE_MVVM_SET_PROPERTY_VALUE(RowDialog,InRowDialog);
}

void UMVVM_LoadSlot::SetDateTime(FDateTime InDateTime)
{
	UE_MVVM_SET_PROPERTY_VALUE(DateTime,InDateTime);
}
