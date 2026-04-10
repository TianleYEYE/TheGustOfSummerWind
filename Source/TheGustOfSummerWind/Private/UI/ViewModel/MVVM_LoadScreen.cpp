// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/SW_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlots.Reset();

	for (int32 SlotIndex = 0; SlotIndex < DefaultSlotCount; ++SlotIndex)
	{
		UMVVM_LoadSlot* LoadSlot = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
		if (!LoadSlot)
		{
			continue;
		}

		LoadSlot->SetLoadSlotName(FString::Printf(TEXT("LoadSlot_%d"), SlotIndex));
		LoadSlots.Add(SlotIndex, LoadSlot);
	}

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::SaveSlotButtonPressed(int32 Slot, UDataTable* EnterDataTable, int32 EnterRowDialog, USoundBase* EnterBackgroundMusic, FDateTime EnterDataTime)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	UMVVM_LoadSlot* const* FoundLoadSlot = LoadSlots.Find(Slot);
	if (!SW_GameMode || !FoundLoadSlot)
	{
		return;
	}

	UMVVM_LoadSlot* LoadSlot = *FoundLoadSlot;
	LoadSlot->SetDataTable(EnterDataTable);
	LoadSlot->SetChapterName(EnterDataTable ? EnterDataTable->GetName() : FString());
	LoadSlot->SlotStatus = Load;
	LoadSlot->SetBackgroundMusic(EnterBackgroundMusic);
	LoadSlot->SetRowDialog(EnterRowDialog);
	LoadSlot->SetDateTime(EnterDataTime);

	SW_GameMode->SaveSlotData(LoadSlot, Slot);
	LoadSlot->InitializeSlot();
}

void UMVVM_LoadScreen::ReadSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
}

void UMVVM_LoadScreen::LoadData()
{
	ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode)
	{
		return;
	}

	for (const TPair<int32, UMVVM_LoadSlot*>& LoadSlotPair : LoadSlots)
	{
		UMVVM_LoadSlot* LoadSlot = LoadSlotPair.Value;
		if (!LoadSlot)
		{
			continue;
		}

		USW_SaveGame* SaveObject = GameMode->GetSaveSlotData(LoadSlot->GetLoadSlotName(), LoadSlotPair.Key);
		if (!SaveObject)
		{
			continue;
		}

		LoadSlot->SlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot->SetChapterName(SaveObject->ChapterName);
		LoadSlot->SetDataTable(SaveObject->DataTable);
		LoadSlot->SetRowDialog(SaveObject->RowDialog);
		LoadSlot->SetDateTime(SaveObject->DateTime);
		LoadSlot->SetBackgroundMusic(SaveObject->BackgroundMusic);
		LoadSlot->InitializeSlot();
	}
}

void UMVVM_LoadScreen::PlayButtonPressed(int32 Slot)
{
	UMVVM_LoadSlot* const* FoundLoadSlot = LoadSlots.Find(Slot);
	if (!FoundLoadSlot)
	{
		return;
	}

	UMVVM_LoadSlot* LoadSlot = *FoundLoadSlot;
	PlayButton.Broadcast(LoadSlot->GetRowDialog(), LoadSlot->GetDataTable(), LoadSlot->GetBackgroundMusic());
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
