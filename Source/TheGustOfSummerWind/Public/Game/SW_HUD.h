// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/AlbumUI/SW_AlbumUI.h"
#include "UI/ContinueUI/SW_ContinueUI.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "UI/InMenuUI/SW_InMenuUI.h"
#include "UI/SettingUI/SW_SettingUI.h"
#include "UI\InGameUI\SW_InGameUI.h"
#include "SW_HUD.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_HUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USW_InGameUI>InGameUIClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_InGameUI>InGameUI;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USW_InMenuUI>InMenuUIClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USW_AlbumUI>InAlbumUIClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AudioPlayer")
	TObjectPtr<AAudioPlayer>AudioPlayer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USW_SettingUI>SettingUIClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<USW_ContinueUI>ContinuUIClass;

	FGameViewportWidgetSlot ViewportSlot;
	UPROPERTY()
	TObjectPtr<UGameViewportSubsystem>Subsystem;
	
	virtual void BeginPlay() override;
	
	void Delegated();

	UFUNCTION()
	void SwitchToNewGame();

	UFUNCTION()
	void SwitchToAlbum();

	UFUNCTION()
	void SwitchToSetting();
	
	UFUNCTION()
	void SwitchToContinue();

	UFUNCTION()
	void CreateInGameUIWidget();

	UFUNCTION()
	void CreateAlbumUIWidget();

	UFUNCTION()
	void CreateSettingUIWidget();

	UFUNCTION()
	void CreateContinueUIWidget();

	

	UPROPERTY()
	TObjectPtr<USW_InMenuUI>inMenuUI;
	
	FTimerHandle TimerHandle;

	bool bIsGameStart=false;
};
