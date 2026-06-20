// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_System.h"

#include "Game/SW_GameMode.h"
#include "Game/SW_ScriptManager.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

namespace
{
constexpr float MinSettingVolume = 0.0f;
constexpr float MaxSettingVolume = 1.0f;
constexpr int32 DefaultScreenWidth = 1920;
constexpr int32 DefaultScreenHeight = 1080;

EWindowMode::Type NormalizeWindowMode(EWindowMode::Type InWindowMode)
{
	switch (InWindowMode)
	{
	case EWindowMode::Fullscreen:
	case EWindowMode::WindowedFullscreen:
	case EWindowMode::Windowed:
		return InWindowMode;
	default:
		return EWindowMode::WindowedFullscreen;
	}
}
}

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
	if (!SettingSlot)
	{
		return;
	}

	SettingSlot->SetTextDisplaySpeed(FMath::Max(0.0f, InTextDisplaySpeed));
	ApplyCurrentSettings(true);
}

void UMVVM_System::MasterVolumeData(float InMasterVolume)
{
	if (!SettingSlot)
	{
		return;
	}

	SettingSlot->SetMasterVolume(FMath::Clamp(InMasterVolume, MinSettingVolume, MaxSettingVolume));
	ApplyCurrentSettings(true);
}

void UMVVM_System::SoundEffectVolumeData(float InSoundEffectVolume)
{
	if (!SettingSlot)
	{
		return;
	}

	SettingSlot->SetSoundEffectVolume(FMath::Clamp(InSoundEffectVolume, MinSettingVolume, MaxSettingVolume));
	ApplyCurrentSettings(true);
}

void UMVVM_System::ConversationalVoiceVolumeData(float InConversationalVoice)
{
	if (!SettingSlot)
	{
		return;
	}

	SettingSlot->SetConversationalVoice(FMath::Clamp(InConversationalVoice, MinSettingVolume, MaxSettingVolume));
	ApplyCurrentSettings(true);
}

void UMVVM_System::ScreenResolutionData(FIntPoint InScreenResolution)
{
	if (!SettingSlot)
	{
		return;
	}

	if (!IsSixteenByNineResolution(InScreenResolution))
	{
		InScreenResolution = FIntPoint(DefaultScreenWidth, DefaultScreenHeight);
	}

	SettingSlot->SetScreenResolution(InScreenResolution);
	ApplyCurrentSettings(true);
}

void UMVVM_System::WindowModeData(TEnumAsByte<EWindowMode::Type> InWindowMode)
{
	if (!SettingSlot)
	{
		return;
	}

	SettingSlot->SetWindowMode(NormalizeWindowMode(InWindowMode.GetValue()));
	ApplyCurrentSettings(true);
}

void UMVVM_System::ApplySoundClassVolume(USoundMix* InSoundMix, USoundClass* InSoundClass, float InVolume, bool bApplyToChildren)
{
	if (!InSoundMix || !InSoundClass)
	{
		return;
	}

	UGameplayStatics::PushSoundMixModifier(this, InSoundMix);
	UGameplayStatics::SetSoundMixClassOverride(this, InSoundMix, InSoundClass, FMath::Clamp(InVolume, MinSettingVolume, MaxSettingVolume), 1.0f, 0.0f, bApplyToChildren);
}

void UMVVM_System::ApplyCurrentSettings(bool bSaveAfterApply)
{
	if (!SettingSlot)
	{
		return;
	}

	ApplyVideoSettings();
	ApplyRuntimeSettings();
	ApplyAudioSettings();

	if (bSaveAfterApply)
	{
		SaveSettingSlot();
	}
}

void UMVVM_System::SaveCurrentSettings()
{
	if (!SettingSlot)
	{
		return;
	}

	SaveSettingSlot();
}

void UMVVM_System::SyncSettingSlotFromRuntime()
{
	if (!SettingSlot)
	{
		return;
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		ScriptManager->ApplyRuntimeSettings();
		SettingSlot->SetTextDisplaySpeed(ScriptManager->TextDisplaySpeed);
		SettingSlot->SetMasterVolume(ScriptManager->MasterVolume);
		SettingSlot->SetSoundEffectVolume(ScriptManager->SoundEffectVolume);
		SettingSlot->SetConversationalVoice(ScriptManager->ConversationalVoice);
	}

	if (UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings())
	{
		const FIntPoint RuntimeResolution = GameUserSettings->GetScreenResolution();
		SettingSlot->SetScreenResolution(IsSixteenByNineResolution(RuntimeResolution) ? RuntimeResolution : FIntPoint(DefaultScreenWidth, DefaultScreenHeight));
		SettingSlot->SetWindowMode(NormalizeWindowMode(GameUserSettings->GetFullscreenMode()));
	}
}

TArray<FIntPoint> UMVVM_System::GetSupportedSixteenByNineResolutions() const
{
	TArray<FIntPoint> SupportedResolutions;
	TArray<FIntPoint> FilteredResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);

	for (const FIntPoint& Resolution : SupportedResolutions)
	{
		if (IsSixteenByNineResolution(Resolution))
		{
			FilteredResolutions.AddUnique(Resolution);
		}
	}

	if (FilteredResolutions.IsEmpty())
	{
		FilteredResolutions.Add(FIntPoint(DefaultScreenWidth, DefaultScreenHeight));
	}

	return FilteredResolutions;
}

FIntPoint UMVVM_System::GetCurrentScreenResolution() const
{
	if (UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings())
	{
		return GameUserSettings->GetScreenResolution();
	}

	return FIntPoint(DefaultScreenWidth, DefaultScreenHeight);
}

TEnumAsByte<EWindowMode::Type> UMVVM_System::GetCurrentWindowMode() const
{
	if (UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings())
	{
		return GameUserSettings->GetFullscreenMode();
	}

	return EWindowMode::WindowedFullscreen;
}

void UMVVM_System::ApplyVideoSettings() const
{
	if (!SettingSlot)
	{
		return;
	}

	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameUserSettings)
	{
		return;
	}

	FIntPoint Resolution = SettingSlot->GetScreenResolution();
	if (!IsSixteenByNineResolution(Resolution))
	{
		Resolution = FIntPoint(DefaultScreenWidth, DefaultScreenHeight);
	}

	GameUserSettings->SetScreenResolution(Resolution);
	GameUserSettings->SetFullscreenMode(NormalizeWindowMode(SettingSlot->GetWindowMode()));
	GameUserSettings->ApplySettings(false);
	GameUserSettings->ConfirmVideoMode();
}

void UMVVM_System::ApplyAudioSettings()
{
	if (!SettingSlot || !GlobalSoundMix)
	{
		return;
	}

	if (MasterSoundClass)
	{
		ApplySoundClassVolume(GlobalSoundMix, MasterSoundClass, SettingSlot->GetMasterVolume());
	}

	if (SoundEffectSoundClass)
	{
		ApplySoundClassVolume(GlobalSoundMix, SoundEffectSoundClass, SettingSlot->GetSoundEffectVolume());
	}

	if (ConversationalVoiceSoundClass)
	{
		ApplySoundClassVolume(GlobalSoundMix, ConversationalVoiceSoundClass, SettingSlot->GetConversationalVoice());
	}
}

void UMVVM_System::ApplyRuntimeSettings() const
{
	if (!SettingSlot)
	{
		return;
	}

	ASW_ScriptManager* ScriptManager = GetScriptManager();
	if (!ScriptManager)
	{
		return;
	}

	ScriptManager->SetTextDisplaySpeed(SettingSlot->GetTextDisplaySpeed());
	ScriptManager->SetMasterVolume(SettingSlot->GetMasterVolume());
	ScriptManager->SetSoundEffectVolume(SettingSlot->GetSoundEffectVolume());
	ScriptManager->SetConversationalVoice(SettingSlot->GetConversationalVoice());
	ScriptManager->ApplyRuntimeSettings();
}

void UMVVM_System::SaveSettingSlot()
{
	if (ASW_GameMode* SW_GameMode = GetSWGameMode())
	{
		SW_GameMode->SaveSystemData(this);
	}

	if (UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings())
	{
		GameUserSettings->SaveSettings();
	}
}

ASW_GameMode* UMVVM_System::GetSWGameMode() const
{
	return Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
}

ASW_ScriptManager* UMVVM_System::GetScriptManager() const
{
	return Cast<ASW_ScriptManager>(UGameplayStatics::GetActorOfClass(this, ASW_ScriptManager::StaticClass()));
}

void UMVVM_System::LoadData()
{
	ASW_GameMode* GameMode = GetSWGameMode();
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
	SettingSlot->SetScreenResolution(IsSixteenByNineResolution(SaveObject->ScreenResolution) ? SaveObject->ScreenResolution : FIntPoint(DefaultScreenWidth, DefaultScreenHeight));
	SettingSlot->SetWindowMode(NormalizeWindowMode(static_cast<EWindowMode::Type>(SaveObject->WindowMode)));

	ApplyCurrentSettings(false);
}

void UMVVM_System::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}

bool UMVVM_System::IsSixteenByNineResolution(const FIntPoint& Resolution)
{
	if (Resolution.X <= 0 || Resolution.Y <= 0)
	{
		return false;
	}

	constexpr float TargetAspectRatio = 16.0f / 9.0f;
	const float CurrentAspectRatio = static_cast<float>(Resolution.X) / static_cast<float>(Resolution.Y);
	return FMath::IsNearlyEqual(CurrentAspectRatio, TargetAspectRatio, 0.01f);
}
