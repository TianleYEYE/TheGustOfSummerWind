#pragma once

#include "CoreMinimal.h"
#include "SW_CharacterBox.h"
#include "SW_DialogBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataStruct/BacklogEntry.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInGame/BTN_NextDialog.h"
#include "DataStruct/DialogStruct.h"
#include "Components/CanvasPanelSlot.h"
#include "SW_InGameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueRecord, FText, readText, FText, readName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBacklogEntryRecorded, FSW_BacklogEntry, Entry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBacklogCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSetCharacter, FDialogStruct, DialogStruct, FDialogStruct, PreviousDialogStruct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchChapterCG);

UENUM(BlueprintType)
enum class EDialogAdvanceMode : uint8
{
	Manual,
	Auto,
	Skip
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogAdvanceModeChanged, EDialogAdvanceMode, AdvanceMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkipStopped, FText, Reason);

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_InGameUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// --- BindWidget references ---

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// TObjectPtr<UTextBlock> TextBlock_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> TEX_Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USW_CharacterBox> CharacterBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USW_DialogBox> BP_DialogBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBTN_NextDialog> BP_NextDialog;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// TObjectPtr<UBorder> NameBoxBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USW_UIBase> BP_Auto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USW_UIBase> BP_Skip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USW_UIBase> BP_Read;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USW_UIBase> BP_BackLog;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Fade;

	// --- Delegates ---

	UPROPERTY(BlueprintAssignable)
	FSetCharacter SetCharacter;

	UPROPERTY(BlueprintAssignable)
	FSwitchChapterCG SwitchChapterCG;

	UPROPERTY(BlueprintAssignable)
	FDialogueRecord DialogueRecord;

	UPROPERTY(BlueprintAssignable, Category = "Backlog")
	FBacklogEntryRecorded BacklogEntryRecorded;

	UPROPERTY(BlueprintAssignable, Category = "Backlog")
	FBacklogCleared BacklogCleared;

	UPROPERTY(BlueprintAssignable, Category = "Dialog")
	FDialogAdvanceModeChanged AdvanceModeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dialog")
	FSkipStopped SkipStopped;

	// --- BP functions migrated to C++ ---

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void UpdateAutoState();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void UpdateSkipState();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateInGameMenuUI(bool bCondition);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdteLogUI(bool bLogExist);

	UFUNCTION(BlueprintCallable, Category = "Backlog")
	void UpdateTextArray(FText Dialogue, FText Name);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void AddBackLogText();

	UFUNCTION(BlueprintCallable, Category = "Chapter")
	USW_UIBase* UpdateChapter();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void AutoActiveEvent();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SkipActiveEvent();

	// --- Core dialog functions ---

	UFUNCTION()
	void InitializeGame();

	UFUNCTION()
	void PressBTN_NEXT(int InRow);

	UFUNCTION(BlueprintCallable)
	void ReadDialog();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void RequestAdvanceDialog(bool bManualRequest = true);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ToggleAutoMode();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void ToggleSkipMode();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void StopAutoMode();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void StopSkipMode();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SetAdvanceMode(EDialogAdvanceMode NewAdvanceMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog")
	EDialogAdvanceMode GetAdvanceMode() const { return AdvanceMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog")
	bool IsAutoModeActive() const { return AdvanceMode == EDialogAdvanceMode::Auto; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog")
	bool IsSkipModeActive() const { return AdvanceMode == EDialogAdvanceMode::Skip; }

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void MarkDialogueAdvanceLocked(float LockSeconds);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Backlog")
	TArray<FSW_BacklogEntry> GetBacklogEntries() const { return BacklogEntries; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Backlog")
	int32 GetBacklogEntryCount() const { return BacklogEntries.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Backlog")
	bool GetBacklogEntryAt(int32 Index, FSW_BacklogEntry& OutEntry) const;

	UFUNCTION(BlueprintCallable, Category = "Backlog")
	void ClearBacklog();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void PrepareLoadedGameState(int32 LoadedRowDialog, UDataTable* LoadedDataTable, USoundBase* LoadedBackgroundMusic);

	UFUNCTION(BlueprintCallable, Category = "Backlog")
	bool JumpToBacklogEntry(const FSW_BacklogEntry& Entry);

	UFUNCTION(BlueprintCallable, Category = "Backlog")
	bool JumpToBacklogIndex(int32 Index);

	// --- Dialog setter helpers ---
	void UpdateStruct();
	void SetName(FDialogStruct* dialogRow);
	void SetBackground(FDialogStruct* dialogRow);
	void SetMusic(FDialogStruct* dialogRow);
	void SetConversationalVoice(FDialogStruct* dialogRow);

	UFUNCTION()
	void SwitchChapter();

	// --- State ---

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> InGameMenuUISlot;

	FWidgetAnimationDynamicEvent CancelInGameMenuUIEvent;
	FWidgetAnimationDynamicEvent InGameMenuUIEvent;

	bool bIsInGameMenuUIExist = false;
	bool bIsPlayAnimation = false;

	FDialogStruct* PreviousDialogStruct;

	int CurrentIndex;
	int Num_nextDialogButtonClicked = 0;

	bool bIsWhiteOver = false;

	int row;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialog Data")
	TObjectPtr<UDataTable> DialogDataTable;

	FDialogStruct* DialogStruct;

	// --- Auto/Skip config ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Auto")
	float AutoBaseDelay = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Auto")
	float AutoPerCharacterDelay = 0.045f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Auto")
	float AutoVoiceTailDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Auto")
	float AutoMinDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Auto")
	float AutoMaxDelay = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Skip")
	float SkipInterval = 0.12f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog|Skip")
	bool bSkipOnlyReadDialog = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	bool bCompleteTextBeforeAdvance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	float ChapterSwitchAdvanceLockSeconds = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backlog", meta = (ClampMin = "0"))
	int32 MaxBacklogEntries = 300;

protected:
	// --- Animation event callbacks ---
	virtual UWidgetAnimation* GetFadeAnimation() const override { return Fade; }

	UFUNCTION()
	void OnFadeStarted();

	UFUNCTION()
	void OnFadeFinished();

	UFUNCTION()
	void OnInGameMenuUI(bool bIsMenuExist);

	UFUNCTION()
	void OnDialogueRecord(FText readText, FText readName);

	UFUNCTION()
	void OnLog(bool bIsLogExist);

	UFUNCTION()
	void OnSetCharacter(FDialogStruct InDialogStruct, FDialogStruct InPreviousDialogStruct);

	UFUNCTION()
	void OnAutoButtonClicked();

	UFUNCTION()
	void OnSkipButtonClicked();

	UFUNCTION()
	void OnReadButtonClicked();

private:
	USW_UIBase* GetButtonPanelWidget() const;
	void BindLegacyButtonClicks();
	void AdvanceDialogImmediately();
	bool TryStartChapterSwitch();
	bool CanAdvanceDialog() const;
	bool IsAdvanceAnimationPlaying() const;
	void SyncAdvanceModeFromBlueprintButtons();
	bool IsBlueprintModeButtonActivated(FName ButtonName) const;
	void SetBlueprintModeButtonActivated(FName ButtonName, bool bActivated);
	void UpdateModeButtonFeedback() const;
	void DispatchCharacterPortraits(const FDialogStruct& CurrentDialog, const FDialogStruct& PreviousDialog);
	void UpdateModeButtonVisuals();
	void ConfigureLegacyBacklogEntryWidgetJumps();
	void LoadPersistentDialogState();
	FName GetCurrentDialogReadKey() const;
	bool IsCurrentDialogRead() const;
	FName MarkCurrentDialogRead();
	bool AddBacklogEntry(const FDialogStruct& InDialogStruct, FName DialogKey);
	float CalculateAutoDelay(const FDialogStruct& InDialogStruct) const;
	void RefreshAutoAdvanceTime(const FDialogStruct& InDialogStruct);

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Dialog", meta = (AllowPrivateAccess = "true"))
	EDialogAdvanceMode AdvanceMode = EDialogAdvanceMode::Manual;

	TSet<FName> ReadDialogKeys;

	UPROPERTY(Transient)
	TArray<FSW_BacklogEntry> BacklogEntries;

	float NextAutoAdvanceTime = 0.0f;
	float NextSkipAdvanceTime = 0.0f;
	float AdvanceLockedUntilTime = 0.0f;
	bool bIsSyncingBlueprintModeState = false;
};
