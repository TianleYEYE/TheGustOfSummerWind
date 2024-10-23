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

void UMVVM_LoadSlot::SetDataTable(UDataTable* InDataTable)
{
	UE_MVVM_SET_PROPERTY_VALUE(DataTable,InDataTable);
}

void UMVVM_LoadSlot::SetChapterName(FString InChapterName)
{
	UE_MVVM_SET_PROPERTY_VALUE(ChapterName,InChapterName);
}

void UMVVM_LoadSlot::SetRowDialog(int32 InRowDialog)
{
	UE_MVVM_SET_PROPERTY_VALUE(RowDialog,InRowDialog);
}
