// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_GameMode.h"

#include "Game/SW_HUD.h"
#include "Game/SW_Pawn.h"
#include "Game/SW_PlayerController.h"
#include "Game/SW_SaveGame.h"
#include "Game/SW_SystemData.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "UI/ViewModel/MVVM_System.h"

ASW_GameMode::ASW_GameMode()
{
	HUDClass = ASW_HUD::StaticClass();
	PlayerControllerClass = ASW_PlayerController::StaticClass();
	DefaultPawnClass = ASW_Pawn::StaticClass();
	LoadScreenSaveGameClass = USW_SaveGame::StaticClass();
	SystemSaveGameClass = USW_SystemData::StaticClass();
}

void ASW_GameMode::SaveSystemData(UMVVM_System* MVVMSystem)
{
	USW_SystemData* SystemDataSaveGame = LoadOrCreateSystemData();
	if (!SystemDataSaveGame || !MVVMSystem || !MVVMSystem->GetSettingSlot())
	{
		return;
	}

	SystemDataSaveGame->CGSlotData.Reset();
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
	SystemDataSaveGame->ScreenResolution = MVVMSystem->GetSettingSlot()->GetScreenResolution();
	SystemDataSaveGame->WindowMode = MVVMSystem->GetSettingSlot()->GetWindowMode();

	SaveSystemDataObject(SystemDataSaveGame);
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
	LoadScreenSaveGame->RowDialog = FMath::Max(0, LoadSlot->GetRowDialog());
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
	return LoadOrCreateSystemData();
}

bool ASW_GameMode::IsDialogKeyRead(const FName DialogKey) const
{
	if (DialogKey.IsNone())
	{
		return false;
	}

	const USW_SystemData* SystemData = LoadOrCreateSystemData();
	return SystemData && SystemData->ReadDialogKeys.Contains(DialogKey);
}

bool ASW_GameMode::AddReadDialogKey(const FName DialogKey) const
{
	if (DialogKey.IsNone())
	{
		return false;
	}

	USW_SystemData* SystemData = LoadOrCreateSystemData();
	if (!SystemData)
	{
		return false;
	}

	if (SystemData->ReadDialogKeys.Contains(DialogKey))
	{
		return true;
	}

	SystemData->ReadDialogKeys.Add(DialogKey);
	SaveSystemDataObject(SystemData);
	return true;
}

USW_SystemData* ASW_GameMode::LoadOrCreateSystemData() const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(TEXT("SystemData"), 0))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(TEXT("SystemData"), 0);
	}
	else
	{
		UClass* SaveGameClass = SystemSaveGameClass ? SystemSaveGameClass.Get() : USW_SystemData::StaticClass();
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	}

	return Cast<USW_SystemData>(SaveGameObject);
}

void ASW_GameMode::SaveSystemDataObject(USW_SystemData* SystemData) const
{
	if (!SystemData)
	{
		return;
	}

	UGameplayStatics::SaveGameToSlot(SystemData, TEXT("SystemData"), 0);
}
