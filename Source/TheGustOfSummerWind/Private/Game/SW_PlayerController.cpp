// Fill out your copyright notice in the Description page of Project Settings.


#include "TheGustOfSummerWind\Public\Game\SW_PlayerController.h"

#include "Kismet/GameplayStatics.h"


void ASW_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	bShowMouseCursor=true;
	LastKeyPressTime = -CooldownTime; // 确保可以在第一次按键时触发
	
	
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
	if (CanPressKey())  // 检查按键是否可以被触发
	{
		LastKeyPressTime = GetWorld()->GetTimeSeconds();  // 更新按键时间
		bIsOnCooldown = true;  // 设置冷却标志，防止短时间内重复按键

		HUD = Cast<ASW_HUD>(GetHUD());

		if (ScriptManager->WidgetState == SettingUI)
		{
			HUD->SettingUI->PlayAnimation(HUD->SettingUI->Fade);
		}
		else if (ScriptManager->WidgetState == AlbumUI)
		{
			HUD->AlbumUI->PlayAnimation(HUD->AlbumUI->Fade);
		}
		else if (ScriptManager->WidgetState == SaveOrLoad)
		{
			HUD->ContinueUI->PlayAnimation(HUD->ContinueUI->ContinueUIFade);
		}
		else if (ScriptManager->WidgetState == InGameSaveOrLoad)
		{
			HUD->ContinueUI->PlayAnimation(HUD->ContinueUI->ContinueUIFade);
		}
		else if (ScriptManager->WidgetState == InGame && !HUD->InGameUI->bIsInGameMenuUIExist )
		{
			HUD->InGameUI->PlayAnimation(HUD->InGameUI->DisplaysInGameMenu);
		}
		else if (ScriptManager->WidgetState == InGame && HUD->InGameUI->bIsInGameMenuUIExist )
		{
			HUD->InGameUI->PlayAnimation(HUD->InGameUI->CancelDisplaysInGameMenu);
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

bool ASW_PlayerController::CanPressKey()
{
	// 当前时间和上次按键时间的时间差
	float TimeSinceLastPress = GetWorld()->GetTimeSeconds() - LastKeyPressTime;

	// 如果冷却时间已经超过
	if (bIsOnCooldown && TimeSinceLastPress >= CooldownTime)
	{
		// 冷却结束，重置冷却状态
		bIsOnCooldown = false;
	}

	// 如果没有在冷却状态，允许按键
	return !bIsOnCooldown;
}

void ASW_PlayerController::QuitGame()
{
	if (QuitGameUI ==nullptr)
	{
		QuitGameUI=CreateWidget<USW_QuitGameUI>(this,BP_QuitGameUIClass);
	}
	if (!QuitGameUI->IsInViewport())
	{
		QuitGameUI->AddToViewport(2);
		QuitGameUI->PlayAnimationReverse(QuitGameUI->Fade);
	}
}