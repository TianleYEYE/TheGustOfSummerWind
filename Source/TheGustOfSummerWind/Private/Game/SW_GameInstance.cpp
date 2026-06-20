// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_GameInstance.h"

#include "GameFramework/GameUserSettings.h"

namespace
{
constexpr int32 TargetWindowWidth = 1920;
constexpr int32 TargetWindowHeight = 1080;
}

void USW_GameInstance::Init()
{
	Super::Init();

	ApplyDefaultWindowSettings();
}

void USW_GameInstance::ApplyDefaultWindowSettings() const
{
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameUserSettings)
	{
		return;
	}

	GameUserSettings->LoadSettings(false);

	bool bUpdatedResolution = false;
	if (!IsSixteenByNine(GameUserSettings->GetScreenResolution()))
	{
		GameUserSettings->SetScreenResolution(FIntPoint(TargetWindowWidth, TargetWindowHeight));
		GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
		bUpdatedResolution = true;
	}

	GameUserSettings->ApplyResolutionSettings(false);

	if (bUpdatedResolution)
	{
		GameUserSettings->ConfirmVideoMode();
		GameUserSettings->SaveSettings();
	}
}

bool USW_GameInstance::IsSixteenByNine(const FIntPoint& Resolution)
{
	if (Resolution.X <= 0 || Resolution.Y <= 0)
	{
		return false;
	}

	constexpr float TargetAspectRatio = 16.0f / 9.0f;
	const float CurrentAspectRatio = static_cast<float>(Resolution.X) / static_cast<float>(Resolution.Y);
	return FMath::IsNearlyEqual(CurrentAspectRatio, TargetAspectRatio, 0.01f);
}
