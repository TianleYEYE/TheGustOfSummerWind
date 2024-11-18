// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SW_HUD.h"
#include "GameFramework/PlayerController.h"
#include "UI/QuitGameUI/SW_QuitGameUI.h"
#include "SW_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInGameMenu,bool,bIsMenuExist);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLog,bool,bIsLogExist);



UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;

	void ReturnOrOpenInGameMenuUI();

	void QuitGame();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USW_UIBase>BP_QuitGameUIClass;
	
	UPROPERTY()
	TObjectPtr<ASW_HUD>HUD;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<ASW_ScriptManager>ScriptManager;

	UPROPERTY()
	TObjectPtr<USW_UIBase>QuitGameUI;

	UPROPERTY(BlueprintAssignable)
	FInGameMenu InGameMenu;

	UPROPERTY(BlueprintAssignable)
	FLog Log;
	
	void BeginPlay() override;

	UPROPERTY(Blueprintable,BlueprintReadWrite)
	bool bIsMenuExist = false;
	UPROPERTY(Blueprintable,BlueprintReadWrite)
	bool bIsLogExist = false;

	void DialogueRecord();
private:
	// 冷却时间（秒）
	float CooldownTime=1.f;
	// 上次按键时间
	float LastKeyPressTime;
	// 是否在冷却中
	bool bIsOnCooldown=false;
	// 检查冷却状态
	bool CanPressKey();
};
