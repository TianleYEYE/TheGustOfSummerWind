// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_PlayerController.h"

#include "Blueprint/GameViewportSubsystem.h"



void ASW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	bShowMouseCursor=true;
	LastKeyPressTime = -CooldownTime;
	
	SetInputMode(FInputModeGameAndUI());

	InputComponent->BindAction("QuitGame",IE_Pressed,this,&ASW_PlayerController::QuitGame);
	InputComponent->BindAction("ReturnOrOpenInGameMenuUI",IE_Released,this,&ASW_PlayerController::ReturnOrOpenInGameMenuUI);
}

void ASW_PlayerController::ReturnOrOpenInGameMenuUI()
{
	if (CanPressKey())
	{
		ASW_HUD *HUD=Cast<ASW_HUD>(GetHUD());
		USW_InGameUI *InGameUI=HUD->InGameUI;
	
		int InGameUIZOrder=HUD->Subsystem->GetWidgetSlot(InGameUI).ZOrder;

		if (InGameUIZOrder==1 && !bIsInGameMenuUIExist)
		{
			InGameUI->GetInGameMenuUI();
			bIsInGameMenuUIExist =true;
			bIsOnCooldown = true;
			LastKeyPressTime = GetWorld()->GetTimeSeconds();
		}
		else if (InGameUIZOrder==1 && bIsInGameMenuUIExist)
		{
			
			InGameUI->CancelInGameMenuUI();
			bIsInGameMenuUIExist =false;
			bIsOnCooldown = true;
			LastKeyPressTime = GetWorld()->GetTimeSeconds();
		}
	
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]
		{
			bIsOnCooldown = false;
		});
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