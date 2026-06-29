// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Game/SW_ScriptManager.h"
#include "Types/SlateEnums.h"
#include "SW_UIBase.generated.h"

class UMVVM_System;
class UMVVM_LoadScreen;
class UButton;
class UComboBoxString;
class UDataTable;
class UPanelWidget;
class USlider;
class UWidget;
class UWidgetAnimation;
class UWidgetSwitcher;
class ASW_HUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayAnimation, bool, bIsPlayingAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayingAnimation, bool, bIsPlayingAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetAnimationPlaying, bool, IsPlaying);

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_UIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BlueprintInializeWidget();
	void BlueprintInializeWidget_Implementation();

	UPROPERTY(EditAnywhere, Category = "AudioPlayer")
	TObjectPtr<ASW_ScriptManager> ScriptManager;

	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayWidgetFade;

	UPROPERTY(BlueprintAssignable)
	FPlayingAnimation PlayingWidgetFade;

	UPROPERTY(BlueprintAssignable)
	FSetAnimationPlaying SetAnimationPlaying;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Backlog")
	bool bJumpToBacklogOnMouseDown = false;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Backlog")
	int32 BacklogIndex = INDEX_NONE;

	// --- BP_UIBase variables migrated to C++ ---

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Button State")
	bool bIsActivated = false;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Animation")
	bool IsPlayAnimation = false;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Slots")
	int32 SlotIndex = 0;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Switcher")
	TObjectPtr<UWidgetSwitcher> ParentSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switcher")
	bool bLoopPageSwitcher = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	bool bRemoveFromParentAfterFade = false;

protected:
	/** Override in subclasses to return the widget-specific fade animation. */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	virtual UWidgetAnimation* GetFadeAnimation() const;

	// --- Utility Functions ---

	ASW_ScriptManager* GetScriptManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Settings")
	UMVVM_System* GetSystemViewModel() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System")
	ASW_HUD* GetHUD() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ViewModel")
	UMVVM_LoadScreen* FindLoadScreenViewModel(UPARAM(DisplayName = "As BP Load Screen View Model") UMVVM_LoadScreen*& AsBPLoadScreenViewModel) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ViewModel")
	UMVVM_System* FindSystemViewModel(UPARAM(DisplayName = "As BP System View Model") UMVVM_System*& AsBPSystemViewModel) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ViewModel")
	UMVVM_System* FindCGViewModel() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Script")
	UDataTable* GetScriptDataTable(UPARAM(DisplayName = "Data Table") UDataTable*& DataTable);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Script")
	int32 GetRowDialog(UPARAM(DisplayName = "Row Dialog") int32& RowDialog);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void PlayGame();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SetWidgetPosition(UUserWidget* Widget);
	void SetWidgetPosition_Implementation(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnRep_ForbiddenInput();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void AnimationStart(UPanelWidget* Target);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void AnimationEnd(UPanelWidget* Target);

	UFUNCTION(BlueprintCallable, Category = "UI")
	static bool AddWidgetToViewportOnce(UUserWidget* Widget, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UI")
	static bool RemoveWidgetFromParentIfValid(UWidget* Widget);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Switcher")
	UWidgetSwitcher* FindPageSwitcher() const;

	UFUNCTION(BlueprintCallable, Category = "Switcher")
	bool SetPageSwitcherActiveIndex(int32 NewIndex, bool bWrap = false);

	UFUNCTION(BlueprintCallable, Category = "Switcher")
	bool SwitchPageByOffset(int32 Offset, bool bWrap = false);

	UFUNCTION(BlueprintCallable, Category = "Switcher")
	void UpdatePageSwitcherNavigationState();

protected:
	UButton* FindButtonInNamedWidget(FName WidgetName, const TArray<FName>& InnerButtonNames) const;

	UFUNCTION()
	void OnReturnButtonClicked();

	UFUNCTION()
	void OnConfirmQuitButtonClicked();

	UFUNCTION()
	void OnCancelQuitButtonClicked();

	UFUNCTION()
	void OnNextPageButtonClicked();

	UFUNCTION()
	void OnPreviousPageButtonClicked();

	UFUNCTION()
	void OnTextSpeedFastClicked();

	UFUNCTION()
	void OnTextSpeedNormalClicked();

	UFUNCTION()
	void OnTextSpeedSlowClicked();

	UFUNCTION()
	void OnTextSpeedTransientClicked();

	UFUNCTION()
	void OnDisplayFullscreenClicked();

	UFUNCTION()
	void OnDisplayWindowedClicked();

	UFUNCTION()
	void OnDisplayWindowedFullscreenClicked();

	UFUNCTION()
	void OnVolumePlusClicked();

	UFUNCTION()
	void OnVolumeMinusClicked();

	UFUNCTION()
	void OnVolumeSliderChanged(float Value);

	UFUNCTION()
	void OnResolutionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnPlayWidgetFade(bool bIsPlayingAnimation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animation")
	void StartFadeUiAnimation();
	void StartFadeUiAnimation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animation")
	void EndedFadeUiAnimation();
	void EndedFadeUiAnimation_Implementation();

	UFUNCTION()
	void OnSetAnimationPlaying(bool IsPlaying);

	UFUNCTION()
	void OnFadeAnimationFinished();

private:
	UObject* FindObjectPropertyOrWidget(UObject* Owner, FName PropertyOrWidgetName) const;
	void BindCommonReturnButton();
	void BindCommonQuitButtons();
	void BindCommonPageSwitcherButtons();
	void BindCommonSettingButtons();
	void InitializeLegacyChildSlots();
	UButton* FindButtonByPath(const TArray<FName>& WidgetPath, const TArray<FName>& ButtonNames) const;
	void SetTextDisplaySpeed(float NewSpeed);
	void SetWindowMode(EWindowMode::Type NewWindowMode);
	void AdjustVolume(float Delta);
	void SetVolume(float NewVolume);
	float GetCurrentVolume() const;
	void InitializeResolutionComboBox(UComboBoxString* ComboBox);
	UButton* FindNextPageButton() const;
	UButton* FindPreviousPageButton() const;
};
