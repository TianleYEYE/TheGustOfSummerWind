// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/InMenuUI/SW_InMenuUI.h"
#include "UI/SettingUI/SW_SettingUI.h"
#include "UI/ViewModel/MVVM_System.h"
#include "SW_HUD.generated.h"


class UMVVM_LoadScreen;
class UMVVM_LoadSlot;
class UDataTable;
class USoundBase;
class USW_InGameUI;
class USW_LoadingUI;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUICollectionInitialized);

UENUM(BlueprintType)
enum class ESWSaveLoadMode : uint8
{
	LoadFromMenu,
	LoadFromInGame,
	SaveFromInGame
};

UENUM()
enum class ESWPendingInGameTransition : uint8
{
	None,
	NewGame,
	LoadFromMenu,
	LoadFromInGame
};

UENUM()
enum class ESWTransitionPhase : uint8
{
	None,
	FadingOutContinue,
	FadingOutMenu,
	FadingInLoading,
	FadingOutLoading,
	FadingInInGame
};
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_HUD : public AHUD
{
	GENERATED_BODY()
public:
	ASW_HUD();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen>LoadScreenViewModelClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen>LoadScreenViewModel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_System>SystemViewModelClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_System>SystemViewModel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_InMenuUI>Menu;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_InGameUI>InGameUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UIBase>ContinueUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UIBase>AlbumUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UIBase>SettingUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<USW_LoadingUI> LoadingUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USW_InMenuUI>MenuClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_InGameUI> InGameUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_UIBase> ContinueUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_UIBase> AlbumUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_UIBase> SettingUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_LoadingUI> LoadingUIClass;

	UFUNCTION()
	void HandleNewGameTransition();

	UFUNCTION()
	void HandleContinueTransition();

	UFUNCTION()
	void HandleAlbumTransition();

	UFUNCTION()
	void HandleSettingTransition();

	UFUNCTION()
	void HandleOverlayFadeComplete(bool bIsPlayingAnimation);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void OpenContinueUI(ESWSaveLoadMode InMode);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void CloseContinueUI(bool bRestoreUnderlyingState = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SaveLoad")
	ESWSaveLoadMode GetContinueUIMode() const { return ContinueUIMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SaveLoad")
	bool IsContinueUISaveMode() const { return ContinueUIMode == ESWSaveLoadMode::SaveFromInGame; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SaveLoad")
	bool IsContinueUIOpenedFromInGame() const { return ContinueUIMode != ESWSaveLoadMode::LoadFromMenu; }

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SetPendingContinueSlotIndex(int32 InSlotIndex) { PendingContinueSlotIndex = InSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SaveLoad")
	int32 GetPendingContinueSlotIndex() const { return PendingContinueSlotIndex; }

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool SaveCurrentGameToSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool LoadGameFromSlot(int32 SlotIndex);

private:
	void InitializeViewModels();
	void CreateMenu();
	void BindMenuButtons();
	void BindFadeCallback(USW_UIBase* Widget);
	void SetOverlayWidget(TObjectPtr<USW_UIBase>& Storage, TSubclassOf<USW_UIBase> WidgetClass, EWidgetStatus NewWidgetState, int32 ZOrder = 1);
	void EnsureInGameUI();
	void EnsureLoadingUI();
	void RefreshContinueUIState() const;
	ASW_ScriptManager* GetScriptManager() const;
	void StartInGameTransition(ESWPendingInGameTransition InTransitionType, int32 InRowDialog, UDataTable* InDataTable, USoundBase* InBackgroundMusic);
	void ContinuePendingTransition();
	void BeginLoadingFadeIn();
	void BeginLoadingFadeOut();
	void PreparePendingInGameState();
	void BeginInGameFadeIn();
	void CompleteInGameTransition();
	void ClearPendingTransition();

	UPROPERTY(Transient)
	ESWSaveLoadMode ContinueUIMode = ESWSaveLoadMode::LoadFromMenu;

	UPROPERTY(Transient)
	int32 PendingContinueSlotIndex = INDEX_NONE;

	UPROPERTY(Transient)
	ESWPendingInGameTransition PendingInGameTransition = ESWPendingInGameTransition::None;

	UPROPERTY(Transient)
	ESWTransitionPhase TransitionPhase = ESWTransitionPhase::None;

	UPROPERTY(Transient)
	int32 PendingRowDialog = 0;

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> PendingDataTable;

	UPROPERTY(Transient)
	TObjectPtr<USoundBase> PendingBackgroundMusic;
};
