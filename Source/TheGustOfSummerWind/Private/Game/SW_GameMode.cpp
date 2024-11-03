// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_GameMode.h"

#include "Game/SW_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "UI/ViewModel/MVVM_System.h"

void ASW_GameMode::SaveSystemData(UMVVM_System* MVVMSystem)
{
	if (UGameplayStatics::DoesSaveGameExist(FString("SystemData"), 0))
	{
		UGameplayStatics::DeleteGameInSlot(FString("SystemData"), 0);
	}

	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SystemSaveGameClass);
	USW_SystemData* SystemDataSaveGame = Cast<USW_SystemData>(SaveGameObject);

	for (const TPair<int32, UMVVM_CGSlot*>& CGSlotPair : MVVMSystem->GetCGSlots())
	{
		UMVVM_CGSlot* CGSlot = CGSlotPair.Value;
		if (CGSlot)
		{
			SystemDataSaveGame->CGSlotData.Add(CGSlotPair.Key, FCGSlotData{ CGSlot->GetCGSlotName(), CGSlot->SlotStatus });
		}
	}

	// 确保每次都从 SettingSlot 获取最新的值
	SystemDataSaveGame->TextDisplaySpeed = MVVMSystem->GetSettingSlot()->GetTextDisplaySpeed();
	SystemDataSaveGame->MasterVolume = MVVMSystem->GetSettingSlot()->GetMasterVolume();
	SystemDataSaveGame->SoundEffectVolume = MVVMSystem->GetSettingSlot()->GetSoundEffectVolume();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("TextDisplaySpeed : %f, MasterVolume: %f"), SystemDataSaveGame->TextDisplaySpeed, SystemDataSaveGame->MasterVolume));

	// 使用固定的保存名称或添加一个方法获取名称
	FString SaveSlotName = TEXT("SystemData");
	UGameplayStatics::SaveGameToSlot(SystemDataSaveGame, SaveSlotName, 0);

	// 调试信息
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Saving CGSlot Data with updated TextDisplaySpeed and MasterVolume"));
}

void ASW_GameMode::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(),SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(),SlotIndex);
	}
	USaveGame * SaveGameObject =UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	USW_SaveGame * LoadScreenSaveGame = Cast<USW_SaveGame>(SaveGameObject);
	LoadScreenSaveGame -> DataTable = LoadSlot -> GetDataTable();
	LoadScreenSaveGame -> BackgroundMusic = LoadSlot ->GetBackgroundMusic();
	LoadScreenSaveGame -> ChapterName = LoadSlot ->GetChapterName();
	LoadScreenSaveGame -> RowDialog = LoadSlot ->GetRowDialog();
	LoadScreenSaveGame -> DateTime = LoadSlot ->GetDateTime();
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

USW_SystemData* ASW_GameMode::GetCGSlotData() const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist("SystemData",0))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot("SystemData",0);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(SystemSaveGameClass);
	}
	USW_SystemData* SaveGame = Cast<USW_SystemData>(SaveGameObject);
	// 加载时
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Loading CGSlot Data"));
	return SaveGame;
}
