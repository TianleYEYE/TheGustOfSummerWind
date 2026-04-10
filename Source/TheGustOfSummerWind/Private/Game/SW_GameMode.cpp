// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_GameMode.h"

#include "Game/SW_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "UI/ViewModel/MVVM_System.h"

void ASW_GameMode::SaveSystemData(UMVVM_System* MVVMSystem)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SystemSaveGameClass);
	USW_SystemData* SystemDataSaveGame = Cast<USW_SystemData>(SaveGameObject);
	if (!SystemDataSaveGame || !MVVMSystem || !MVVMSystem->GetSettingSlot())
	{
		return;
	}

	for (const TPair<int32, UMVVM_CGSlot*>& CGSlotPair : MVVMSystem->GetCGSlots())
	{
		UMVVM_CGSlot* CGSlot = CGSlotPair.Value;
		if (CGSlot)
		{
			SystemDataSaveGame->CGSlotData.Add(CGSlotPair.Key, FCGSlotData{CGSlot->GetCGSlotName(), CGSlot->SlotStatus});
		}
	}

	SystemDataSaveGame->TextDisplaySpeed = MVVMSystem->GetSettingSlot()->GetTextDisplaySpeed();
	SystemDataSaveGame->MasterVolume = MVVMSystem->GetSettingSlot()->GetMasterVolume();
	SystemDataSaveGame->SoundEffectVolume = MVVMSystem->GetSettingSlot()->GetSoundEffectVolume();
	SystemDataSaveGame->ConversationalVoice = MVVMSystem->GetSettingSlot()->GetConversationalVoice();

	const FString SaveSlotName = TEXT("SystemData");
	UGameplayStatics::SaveGameToSlot(SystemDataSaveGame, SaveSlotName, 0);
}

void ASW_GameMode::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	USW_SaveGame* LoadScreenSaveGame = Cast<USW_SaveGame>(SaveGameObject);
	if (!LoadScreenSaveGame || !LoadSlot)
	{
		return;
	}

	LoadScreenSaveGame->DataTable = LoadSlot->GetDataTable();
	LoadScreenSaveGame->BackgroundMusic = LoadSlot->GetBackgroundMusic();
	LoadScreenSaveGame->ChapterName = LoadSlot->GetChapterName();
	LoadScreenSaveGame->RowDialog = LoadSlot->GetRowDialog();
	LoadScreenSaveGame->DateTime = LoadSlot->GetDateTime();
	LoadScreenSaveGame->SaveSlotStatus = Load;

	if (LoadScreenSaveGame->DataTable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("DataTable : %s , RowDialog: %d"), *LoadScreenSaveGame->DataTable->GetName(), LoadScreenSaveGame->RowDialog));
	}

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

USW_SaveGame* ASW_GameMode::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	return Cast<USW_SaveGame>(SaveGameObject);
}

USW_SystemData* ASW_GameMode::GetCGSlotData() const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(TEXT("SystemData"), 0))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(TEXT("SystemData"), 0);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(SystemSaveGameClass);
	}

	return Cast<USW_SystemData>(SaveGameObject);
}
