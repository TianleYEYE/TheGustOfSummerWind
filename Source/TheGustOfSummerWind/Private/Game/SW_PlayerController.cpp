// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_PlayerController.h"
#include "GameFramework/GameUserSettings.h"

#include "Kismet/GameplayStatics.h"


void ASW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	bShowMouseCursor = true;
	LastKeyPressTime = -CooldownTime;

	SetInputMode(FInputModeGameAndUI());

	AActor* TempActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASW_ScriptManager::StaticClass());
	if (TempActor)
	{
		ScriptManager = Cast<ASW_ScriptManager>(TempActor);
	}

	InputComponent->BindAction("QuitGame", IE_Pressed, this, &ThisClass::QuitGame);
	InputComponent->BindAction("ReturnOrOpenInGameMenuUI", IE_Released, this, &ThisClass::ReturnOrOpenInGameMenuUI);
	InputComponent->BindAction("DialogueRecord", IE_Pressed, this, &ThisClass::DialogueRecord);
}

void ASW_PlayerController::ReturnOrOpenInGameMenuUI()
{
	if (!ScriptManager)
	{
		return;
	}

	if (CanPressKey())
	{
		LastKeyPressTime = GetWorld()->GetTimeSeconds();
		bIsOnCooldown = true;
		HUD = Cast<ASW_HUD>(GetHUD());

		if (ScriptManager->WidgetState == SettingUI || ScriptManager->WidgetState == InGameSetting)
		{
			HUD->SettingUI->PlayWidgetFade.Broadcast(true);
			UGameUserSettings::GetGameUserSettings()->SaveSettings();
		}
		else if (ScriptManager->WidgetState == AlbumUI)
		{
			HUD->AlbumUI->PlayWidgetFade.Broadcast(true);
		}
		else if (ScriptManager->WidgetState == SaveOrLoad || ScriptManager->WidgetState == InGameSaveOrLoad)
		{
			HUD->ContinueUI->PlayWidgetFade.Broadcast(true);
		}
		else if (ScriptManager->WidgetState == InGame)
		{
			InGameMenu.Broadcast(bIsMenuExist);
		}
		else if (ScriptManager->WidgetState == BackLog && bIsLogExist)
		{
			Log.Broadcast(bIsLogExist);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Still on cooldown, cannot return to menu yet."));
	}
}

void ASW_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	LastKeyPressTime = GetWorld()->GetTimeSeconds();
}

void ASW_PlayerController::DialogueRecord()
{
	if (!ScriptManager)
	{
		return;
	}

	if (CanPressKey() && ScriptManager->WidgetState == InGame)
	{
		LastKeyPressTime = GetWorld()->GetTimeSeconds();
		bIsOnCooldown = true;

		if (!bIsLogExist)
		{
			Log.Broadcast(bIsLogExist);
		}
	}
}

bool ASW_PlayerController::CanPressKey()
{
	const float TimeSinceLastPress = GetWorld()->GetTimeSeconds() - LastKeyPressTime;
	if (bIsOnCooldown && TimeSinceLastPress >= CooldownTime)
	{
		bIsOnCooldown = false;
	}

	return !bIsOnCooldown;
}

void ASW_PlayerController::QuitGame()
{
	if (QuitGameUI == nullptr)
	{
		QuitGameUI = CreateWidget<USW_UIBase>(this, BP_QuitGameUIClass);
	}

	if (!QuitGameUI->IsInViewport())
	{
		QuitGameUI->AddToViewport(2);
	}
}
