// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_System.h"

#include "Game/SW_GameMode.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_System::InitializeLoadSlots()
{
	CGSlot_0 = NewObject<UMVVM_CGSlot>(this,CGSlotViewModelClass);
	CGSlot_0->SetCGSlotName(FString("CGSlot_0"));
	CGSlots.Add(0,CGSlot_0);
	CGSlot_1 = NewObject<UMVVM_CGSlot>(this,CGSlotViewModelClass);
	CGSlot_1->SetCGSlotName(FString("CGSlot_1"));
	CGSlots.Add(1,CGSlot_1);
	CGSlot_2 = NewObject<UMVVM_CGSlot>(this,CGSlotViewModelClass);
	CGSlot_2->SetCGSlotName(FString("CGSlot_2"));
	CGSlots.Add(2,CGSlot_2);
	CGSlot_3 = NewObject<UMVVM_CGSlot>(this,CGSlotViewModelClass);
	CGSlot_3->SetCGSlotName(FString("CGSlot_3"));
	CGSlots.Add(3,CGSlot_3);
	CGSlot_4 = NewObject<UMVVM_CGSlot>(this,CGSlotViewModelClass);
	CGSlot_4->SetCGSlotName(FString("CGSlot_4"));
	CGSlots.Add(4,CGSlot_4);
	CGSlot_5 = NewObject<UMVVM_CGSlot>(this,CGSlotViewModelClass);
	CGSlot_5->SetCGSlotName(FString("CGSlot_5"));
	CGSlots.Add(5,CGSlot_5);

	SettingSlot = NewObject<UMVVM_SettingSlot>(this,SettingSlotViewModelClass);

	
	SetNumLoadSlots(CGSlots.Num());
}

UMVVM_CGSlot* UMVVM_System::GetCGSlotViewModelByIndex(int32 index)
{
	return CGSlots.FindChecked(index);
}

void UMVVM_System::CGSlotUnlock(int32 Slot)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));

	// 解锁当前 CGSlot
	CGSlots[Slot]->SlotStatus = Unlocked;

	// 更新所有 CGSlot 数据
	SW_GameMode->SaveSystemData(this);

	// 初始化 CGSlot
	CGSlots[Slot]->InitializeSlot();
}

void UMVVM_System::TextDisplaySpeedData(float InTextDisplaySpeed)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));

	SettingSlot->SetTextDisplaySpeed(InTextDisplaySpeed);

	SW_GameMode ->SaveSystemData(this);
}

void UMVVM_System::MasterVolumeData(float InMasterVolume)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));

	SettingSlot ->SetMasterVolume(InMasterVolume);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,FString::Printf(TEXT("TextDisplaySpeed : %f"),));
	
	SW_GameMode ->SaveSystemData(this);
}

void UMVVM_System::SoundEffectVolumeData(float InSoundEffectVolume)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));

	SettingSlot ->SetSoundEffectVolume(InSoundEffectVolume);

	SW_GameMode ->SaveSystemData(this);
}

void UMVVM_System::ConversationalVoiceVolumeData(float InConversationalVoice)
{
	ASW_GameMode* SW_GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));

	SettingSlot ->SetConversationalVoice(InConversationalVoice);

	SW_GameMode ->SaveSystemData(this);
}


void UMVVM_System::LoadData()
{
	ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	USW_SystemData* SaveObject = GameMode->GetCGSlotData();

	if (!SaveObject) return; // 确保 SaveObject 有效

	for (const TPair<int32, UMVVM_CGSlot*>& CGSlotPair : CGSlots)
	{
		UMVVM_CGSlot* CGSlot = CGSlotPair.Value;
		if (CGSlot)
		{
			if (const FCGSlotData* SlotData = SaveObject->CGSlotData.Find(CGSlotPair.Key))
			{
				CGSlot->SetCGSlotName(SlotData->Name);
				CGSlot->SlotStatus = SlotData->AlbumStatus;
				CGSlot->InitializeSlot();
			}
		}
	}
	GetSettingSlot()->SetMasterVolume(SaveObject->MasterVolume);
	GetSettingSlot()->SetTextDisplaySpeed(SaveObject->TextDisplaySpeed);
	GetSettingSlot()->SetSoundEffectVolume(SaveObject->SoundEffectVolume);
	GetSettingSlot()->SetConversationalVoice(SaveObject->ConversationalVoice);
}

void UMVVM_System::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots,InNumLoadSlots);
}
