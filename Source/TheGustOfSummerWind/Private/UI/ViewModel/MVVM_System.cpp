// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_System.h"

#include "Game/SW_GameMode.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_System::InitializeLoadSlots()
{
	CGSlots.Reset();

	for (int32 SlotIndex = 0; SlotIndex < DefaultCGSlotCount; ++SlotIndex)
	{
		UMVVM_CGSlot* CGSlot = NewObject<UMVVM_CGSlot>(this, CGSlotViewModelClass);
		if (!CGSlot)
		{
			continue;
		}

		CGSlot->SetCGSlotName(FString::Printf(TEXT("CGSlot_%d"), SlotIndex));
		CGSlots.Add(SlotIndex, CGSlot);
	}

	SettingSlot = NewObject<UMVVM_SettingSlot>(this, SettingSlotViewModelClass);
	SetNumLoadSlots(CGSlots.Num());
}

UMVVM_CGSlot* UMVVM_System::GetCGSlotViewModelByIndex(int32 index)
{
	return CGSlots.FindChecked(index);
}

void UMVVM_System::CGSlotUnlock(int32 Slot)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	UMVVM_CGSlot* const* FoundCGSlot = CGSlots.Find(Slot);
	if (!SW_GameMode || !FoundCGSlot)
	{
		return;
	}

	UMVVM_CGSlot* CGSlot = *FoundCGSlot;
	CGSlot->SlotStatus = Unlocked;
	SW_GameMode->SaveSystemData(this);
	CGSlot->InitializeSlot();
}

void UMVVM_System::TextDisplaySpeedData(float InTextDisplaySpeed)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!SW_GameMode || !SettingSlot)
	{
		return;
	}

	SettingSlot->SetTextDisplaySpeed(InTextDisplaySpeed);
	SW_GameMode->SaveSystemData(this);
}

void UMVVM_System::MasterVolumeData(float InMasterVolume)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!SW_GameMode || !SettingSlot)
	{
		return;
	}

	SettingSlot->SetMasterVolume(InMasterVolume);
	SW_GameMode->SaveSystemData(this);
}

void UMVVM_System::SoundEffectVolumeData(float InSoundEffectVolume)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!SW_GameMode || !SettingSlot)
	{
		return;
	}

	SettingSlot->SetSoundEffectVolume(InSoundEffectVolume);
	SW_GameMode->SaveSystemData(this);
}

void UMVVM_System::ConversationalVoiceVolumeData(float InConversationalVoice)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!SW_GameMode || !SettingSlot)
	{
		return;
	}

	SettingSlot->SetConversationalVoice(InConversationalVoice);
	SW_GameMode->SaveSystemData(this);
}

void UMVVM_System::LoadData()
{
	ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode || !SettingSlot)
	{
		return;
	}

	USW_SystemData* SaveObject = GameMode->GetCGSlotData();
	if (!SaveObject)
	{
		return;
	}

	for (const TPair<int32, UMVVM_CGSlot*>& CGSlotPair : CGSlots)
	{
		UMVVM_CGSlot* CGSlot = CGSlotPair.Value;
		if (!CGSlot)
		{
			continue;
		}

		if (const FCGSlotData* SlotData = SaveObject->CGSlotData.Find(CGSlotPair.Key))
		{
			CGSlot->SetCGSlotName(SlotData->Name);
			CGSlot->SlotStatus = SlotData->AlbumStatus;
			CGSlot->InitializeSlot();
		}
	}

	SettingSlot->SetMasterVolume(SaveObject->MasterVolume);
	SettingSlot->SetTextDisplaySpeed(SaveObject->TextDisplaySpeed);
	SettingSlot->SetSoundEffectVolume(SaveObject->SoundEffectVolume);
	SettingSlot->SetConversationalVoice(SaveObject->ConversationalVoice);
}

void UMVVM_System::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
