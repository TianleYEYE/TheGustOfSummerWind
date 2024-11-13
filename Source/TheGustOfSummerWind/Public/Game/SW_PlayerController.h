// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SW_HUD.h"
#include "GameFramework/PlayerController.h"
#include "UI/QuitGameUI/SW_QuitGameUI.h"
#include "SW_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInGameMenu,bool,bIsMenuExist);


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
	TSubclassOf<USW_QuitGameUI>BP_QuitGameUIClass;
	
	UPROPERTY()
	TObjectPtr<ASW_HUD>HUD;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<ASW_ScriptManager>ScriptManager;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USW_QuitGameUI>QuitGameUI;

	UPROPERTY(BlueprintAssignable)
	FInGameMenu InGameMenu;
	
	void BeginPlay() override;

	UPROPERTY(Blueprintable,BlueprintReadWrite)
	bool bIsMenuExist = false;

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
