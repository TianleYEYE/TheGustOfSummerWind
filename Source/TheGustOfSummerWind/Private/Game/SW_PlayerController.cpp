// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_PlayerController.h"

#include "Kismet/GameplayStatics.h"


void ASW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	bShowMouseCursor=true;
	LastKeyPressTime = -CooldownTime;
	
	SetInputMode(FInputModeGameAndUI());
	auto TempActor = UGameplayStatics::GetActorOfClass(GetWorld(),ScriptManager->StaticClass());
	if (TempActor)
	{
		ScriptManager= Cast<ASW_ScriptManager>(TempActor);
	}

	InputComponent->BindAction("QuitGame",IE_Pressed,this,&ASW_PlayerController::QuitGame);
	InputComponent->BindAction("ReturnOrOpenInGameMenuUI",IE_Released,this,&ASW_PlayerController::ReturnOrOpenInGameMenuUI);
}

void ASW_PlayerController::ReturnOrOpenInGameMenuUI()
{
	if (CanPressKey())
	{
		HUD=Cast<ASW_HUD>(GetHUD());
		if (ScriptManager->WidgetState != Menu && ScriptManager->WidgetState != InGameSaveOrLoad && ScriptManager->WidgetState != InGame)
		{
			HUD->UICollection->UISwitcher->SetActiveWidgetIndex(0);
		}
		else if (ScriptManager->WidgetState == InGameSaveOrLoad)
		{
			HUD->UICollection->UISwitcher->SetActiveWidgetIndex(1);
		}
		else if (ScriptManager->WidgetState == InGame && !bIsInGameMenuUIExist)
		{
			HUD->UICollection->BP_InGameUI->PlayAnimation(HUD->UICollection->BP_InGameUI->DisplaysInGameMenu);
			bIsInGameMenuUIExist=true;
		}
		else if (ScriptManager->WidgetState == InGame && bIsInGameMenuUIExist)
		{
			HUD->UICollection->BP_InGameUI->PlayAnimation(HUD->UICollection->BP_InGameUI->CancelDisplaysInGameMenu);
			bIsInGameMenuUIExist=false;
		}
	}
	
}
bool ASW_PlayerController::CanPressKey() const
{
	return !bIsOnCooldown && GetWorld()->GetTimeSeconds() - LastKeyPressTime >= CooldownTime;
}

void ASW_PlayerController::QuitGame()
{
	USW_QuitGameUI*quitGameUI=CreateWidget<USW_QuitGameUI>(this,BP_QuitGameUIClass);
	
	quitGameUI->AddToViewport(2);
	
}