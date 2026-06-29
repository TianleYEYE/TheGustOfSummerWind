// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SW_HUD.h"
#include "GameFramework/PlayerController.h"
#include "UI/SW_UIBase.h"
#include "SW_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInGameMenu, bool, bIsMenuExist);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLog, bool, bIsLogExist);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAlbumUI);

UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	void ReturnOrOpenInGameMenuUI();
	void QuitGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USW_UIBase> BP_QuitGameUIClass;

	UPROPERTY()
	TObjectPtr<ASW_HUD> HUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ASW_ScriptManager> ScriptManager;

	UPROPERTY()
	TObjectPtr<USW_UIBase> QuitGameUI;

	UPROPERTY(BlueprintAssignable)
	FInGameMenu InGameMenu;

	UPROPERTY(BlueprintAssignable)
	FLog Log;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	bool bIsMenuExist = false;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	bool bIsLogExist = false;

	void DialogueRecord();

private:
	float CooldownTime = 1.f;
	float LastKeyPressTime;
	bool bIsOnCooldown = false;
	bool CanPressKey();
};
