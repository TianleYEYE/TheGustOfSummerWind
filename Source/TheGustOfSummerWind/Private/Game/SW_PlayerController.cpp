// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_PlayerController.h"


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
	// 	HUD=Cast<ASW_HUD>(GetHUD());
	// 	HUD->widgetIndex=HUD->UICollection->UISwitcher->GetActiveWidgetIndex();
	// 	if (HUD->widgetIndex>0)
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Cyan,FString::Printf(TEXT("Widget Index = %d"),HUD->widgetIndex));
	// 		//HUD->widgetIndex=0;
	// 		HUD->UICollection->UISwitcher->SetActiveWidgetIndex(0);
	// 	}
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