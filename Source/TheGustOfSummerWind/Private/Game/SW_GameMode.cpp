// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_GameMode.h"

#include "Game/SW_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void ASW_GameMode::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(),SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(),SlotIndex);
	}
	USaveGame * SaveGameObject =UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	USW_SaveGame * LoadScreenSaveGame = Cast<USW_SaveGame>(SaveGameObject);
	LoadScreenSaveGame -> DataTable = LoadSlot -> GetDataTable();
	LoadScreenSaveGame -> ChapterName = LoadSlot ->GetChapterName();
	LoadScreenSaveGame -> RowDialog = LoadSlot ->GetRowDialog();
	LoadScreenSaveGame -> SaveSlotStatus= Load;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame,LoadSlot->GetLoadSlotName(),SlotIndex);
}

USW_SaveGame* ASW_GameMode::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SlotName,SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName,SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	USW_SaveGame* SaveGame = Cast<USW_SaveGame>(SaveGameObject);
	return SaveGame;
}
