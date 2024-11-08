// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/SW_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlots.Add(0,LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1,LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlots.Add(2,LoadSlot_2);
	LoadSlot_3 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_3->SetLoadSlotName(FString("LoadSlot_3"));
	LoadSlots.Add(3,LoadSlot_3);
	LoadSlot_4 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_4->SetLoadSlotName(FString("LoadSlot_4"));
	LoadSlots.Add(4,LoadSlot_4);
	LoadSlot_5 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_5->SetLoadSlotName(FString("LoadSlot_5"));
	LoadSlots.Add(5,LoadSlot_5);
	LoadSlot_6 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_6->SetLoadSlotName(FString("LoadSlot_6"));
	LoadSlots.Add(6,LoadSlot_6);
	LoadSlot_7 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_7->SetLoadSlotName(FString("LoadSlot_7"));
	LoadSlots.Add(7,LoadSlot_7);
	LoadSlot_8 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_8->SetLoadSlotName(FString("LoadSlot_8"));
	LoadSlots.Add(8,LoadSlot_8);
	LoadSlot_9 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_9->SetLoadSlotName(FString("LoadSlot_9"));
	LoadSlots.Add(9,LoadSlot_9);
	LoadSlot_10 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_10->SetLoadSlotName(FString("LoadSlot_10"));
	LoadSlots.Add(10,LoadSlot_10);
	LoadSlot_11 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_11->SetLoadSlotName(FString("LoadSlot_11"));
	LoadSlots.Add(11,LoadSlot_11);

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::SaveSlotButtonPressed(int32 Slot,UDataTable *EnterDataTable,int32 EnterRowDialog,USoundBase *EnterBackgroundMusic,FDateTime EnterDataTime)
{
	
	ASW_GameMode *SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetDataTable(EnterDataTable);
	LoadSlots[Slot]->SetChapterName(EnterDataTable->GetName());
	LoadSlots[Slot]->SlotStatus = Load;
	LoadSlots[Slot]->SetBackgroundMusic(EnterBackgroundMusic);
	LoadSlots[Slot]->SetRowDialog(EnterRowDialog);
	LoadSlots[Slot]->SetDateTime(EnterDataTime);
	
	SW_GameMode->SaveSlotData(LoadSlots[Slot],Slot);
	LoadSlots[Slot]->InitializeSlot();
}


void UMVVM_LoadScreen::ReadSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
}

void UMVVM_LoadScreen::LoadData()
{
	ASW_GameMode* GameMode =Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	 for (const TTuple<int32,UMVVM_LoadSlot*> LoadSlot:LoadSlots)
	 {
	 	USW_SaveGame* SaveObject=GameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(),LoadSlot.Key);

	 	UDataTable *DataTable = SaveObject->DataTable;
	 	USoundBase *BackgroundMusic =SaveObject->BackgroundMusic;
	 	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus =SaveObject ->SaveSlotStatus;
	 	int32 RowDialog =SaveObject ->RowDialog;
	 	FString ChapterName = SaveObject->ChapterName;
	 	FDateTime DateTime = SaveObject ->DateTime;

	 	LoadSlot.Value->SlotStatus = SaveSlotStatus;
	 	LoadSlot.Value->SetChapterName(ChapterName);
	 	LoadSlot.Value->SetDataTable(DataTable);
	 	LoadSlot.Value->SetRowDialog(RowDialog);
	 	LoadSlot.Value->SetDateTime(DateTime);
	 	LoadSlot.Value->SetBackgroundMusic(BackgroundMusic);
	 	
 	 	LoadSlot.Value->InitializeSlot();

	 }
}

void UMVVM_LoadScreen::PlayButtonPressed(int32 Slot)
{
	PlayButton.Broadcast(LoadSlots[Slot]->GetRowDialog(),LoadSlots[Slot]->GetDataTable(),LoadSlots[Slot]->GetBackgroundMusic());
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots,InNumLoadSlots);
}

