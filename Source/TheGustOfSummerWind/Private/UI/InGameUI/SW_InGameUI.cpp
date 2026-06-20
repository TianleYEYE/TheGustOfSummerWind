// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_InGameUI.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/AudioComponent.h"
#include "Components/Widget.h"
#include "Game/SW_GameMode.h"
#include "Game/SW_HUD.h"
#include "Game/SW_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/UnrealType.h"

namespace
{
constexpr TCHAR AutoButtonPropertyName[] = TEXT("BP_Auto");
constexpr TCHAR SkipButtonPropertyName[] = TEXT("BP_Skip");
constexpr TCHAR ButtonContainerPropertyName[] = TEXT("BP_ButtonOfInGameUI");

UObject* GetInGameUIObjectPropertyValue(UObject* Owner, const FName PropertyName)
{
	if (!Owner)
	{
		return nullptr;
	}

	const FObjectPropertyBase* ObjectProperty = FindFProperty<FObjectPropertyBase>(Owner->GetClass(), PropertyName);
	return ObjectProperty ? ObjectProperty->GetObjectPropertyValue_InContainer(Owner) : nullptr;
}

bool GetInGameUIBoolPropertyValue(const UObject* Owner, const FName PropertyName)
{
	if (!Owner)
	{
		return false;
	}

	const FBoolProperty* BoolProperty = FindFProperty<FBoolProperty>(Owner->GetClass(), PropertyName);
	return BoolProperty ? BoolProperty->GetPropertyValue_InContainer(Owner) : false;
}

void SetInGameUIBoolPropertyValue(UObject* Owner, const FName PropertyName, const bool bValue)
{
	if (!Owner)
	{
		return;
	}

	if (const FBoolProperty* BoolProperty = FindFProperty<FBoolProperty>(Owner->GetClass(), PropertyName))
	{
		BoolProperty->SetPropertyValue_InContainer(Owner, bValue);
	}
}

void EmptyBlueprintArrayProperty(UObject* Owner, const FName PropertyName, const bool bRemoveWidgetChildren)
{
	if (!Owner)
	{
		return;
	}

	const FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(Owner->GetClass(), PropertyName);
	if (!ArrayProperty)
	{
		return;
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(Owner));
	if (bRemoveWidgetChildren)
	{
		if (const FObjectPropertyBase* ObjectProperty = CastField<FObjectPropertyBase>(ArrayProperty->Inner))
		{
			for (int32 Index = 0; Index < ArrayHelper.Num(); ++Index)
			{
				if (UWidget* Widget = Cast<UWidget>(ObjectProperty->GetObjectPropertyValue(ArrayHelper.GetRawPtr(Index))))
				{
					Widget->RemoveFromParent();
				}
			}
		}
	}

	ArrayHelper.EmptyValues();
}

void ClearLegacyBacklogBlueprintState(UObject* Owner)
{
	EmptyBlueprintArrayProperty(Owner, TEXT("readDialogue"), false);
	EmptyBlueprintArrayProperty(Owner, TEXT("readName"), false);
	EmptyBlueprintArrayProperty(Owner, TEXT("BP_NameAndDialogueArray"), true);
}
}

void USW_InGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeGame();
}

void USW_InGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SyncAdvanceModeFromBlueprintButtons();
	ConfigureLegacyBacklogEntryWidgetJumps();

	if (!CanAdvanceDialog() || IsAdvanceAnimationPlaying())
	{
		return;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float Now = World->GetTimeSeconds();
	if (AdvanceMode == EDialogAdvanceMode::Auto && Now >= NextAutoAdvanceTime)
	{
		RequestAdvanceDialog(false);
	}
	else if (AdvanceMode == EDialogAdvanceMode::Skip && Now >= NextSkipAdvanceTime)
	{
		RequestAdvanceDialog(false);
	}
}

void USW_InGameUI::InitializeGame()
{
	ScriptManager = GetScriptManager();
	LoadPersistentDialogState();
	if (BP_NextDialog)
	{
		BP_NextDialog->EntrustDelegated.RemoveDynamic(this, &USW_InGameUI::PressBTN_NEXT);
		BP_NextDialog->EntrustDelegated.AddUniqueDynamic(this, &USW_InGameUI::PressBTN_NEXT);
	}
}

void USW_InGameUI::PressBTN_NEXT(int32 InRow)
{
	RequestAdvanceDialog(true);
}

void USW_InGameUI::ReadDialog()
{
	RequestAdvanceDialog(false);
}

void USW_InGameUI::RequestAdvanceDialog(const bool bManualRequest)
{
	ScriptManager = GetScriptManager();
	if (!ScriptManager)
	{
		return;
	}

	SyncAdvanceModeFromBlueprintButtons();

	if (bManualRequest)
	{
		SetAdvanceMode(EDialogAdvanceMode::Manual);
	}

	if (!CanAdvanceDialog())
	{
		return;
	}

	const UWorld* World = GetWorld();
	const float Now = World ? World->GetTimeSeconds() : 0.0f;

	if (AdvanceMode == EDialogAdvanceMode::Auto && Now < NextAutoAdvanceTime)
	{
		return;
	}

	if (AdvanceMode == EDialogAdvanceMode::Skip && Now < NextSkipAdvanceTime)
	{
		return;
	}

	if (BP_DialogBox && BP_DialogBox->IsDialogTextRevealing())
	{
		const bool bModeRequestedAdvance = !bManualRequest && AdvanceMode != EDialogAdvanceMode::Manual;
		if (bCompleteTextBeforeAdvance && !bModeRequestedAdvance)
		{
			BP_DialogBox->CompleteDialogTextReveal();
		}
		return;
	}

	if (AdvanceMode == EDialogAdvanceMode::Skip && bSkipOnlyReadDialog && !IsCurrentDialogRead())
	{
		SetAdvanceMode(EDialogAdvanceMode::Manual);
		SkipStopped.Broadcast(FText::FromString(TEXT("Skip stopped at unread dialog.")));
		return;
	}

	if (TryStartChapterSwitch())
	{
		return;
	}

	AdvanceDialogImmediately();
}

void USW_InGameUI::AdvanceDialogImmediately()
{
	ScriptManager = GetScriptManager();
	if (!ScriptManager)
	{
		return;
	}

	UpdateStruct();
	if (!DialogStruct || !PreviousDialogStruct || !BP_DialogBox)
	{
		return;
	}

	BP_DialogBox->SetDialogText(DialogStruct->Dialog);
	SetCharacter.Broadcast(*DialogStruct, *PreviousDialogStruct);

	SetName(DialogStruct);
	SetBackground(DialogStruct);
	SetMusic(DialogStruct);
	SetConversationalVoice(DialogStruct);
	const FName DialogReadKey = MarkCurrentDialogRead();
	const bool bRecordedBacklogEntry = AddBacklogEntry(*DialogStruct, DialogReadKey);
	if (bRecordedBacklogEntry)
	{
		DialogueRecord.Broadcast(DialogStruct->Dialog, DialogStruct->Name);
	}
	RefreshAutoAdvanceTime(*DialogStruct);

	CurrentIndex = 0;
	ScriptManager->rowDialog++;

	if (const UWorld* World = GetWorld())
	{
		NextSkipAdvanceTime = World->GetTimeSeconds() + FMath::Max(0.01f, SkipInterval);
	}
}

void USW_InGameUI::ToggleAutoMode()
{
	SetAdvanceMode(AdvanceMode == EDialogAdvanceMode::Auto ? EDialogAdvanceMode::Manual : EDialogAdvanceMode::Auto);
}

void USW_InGameUI::ToggleSkipMode()
{
	SetAdvanceMode(AdvanceMode == EDialogAdvanceMode::Skip ? EDialogAdvanceMode::Manual : EDialogAdvanceMode::Skip);
}

void USW_InGameUI::StopAutoMode()
{
	if (AdvanceMode == EDialogAdvanceMode::Auto)
	{
		SetAdvanceMode(EDialogAdvanceMode::Manual);
	}
	else
	{
		SetBlueprintModeButtonActivated(AutoButtonPropertyName, false);
		UpdateModeButtonFeedback();
	}
}

void USW_InGameUI::StopSkipMode()
{
	if (AdvanceMode == EDialogAdvanceMode::Skip)
	{
		SetAdvanceMode(EDialogAdvanceMode::Manual);
	}
	else
	{
		SetBlueprintModeButtonActivated(SkipButtonPropertyName, false);
		UpdateModeButtonFeedback();
	}
}

void USW_InGameUI::SetAdvanceMode(const EDialogAdvanceMode NewAdvanceMode)
{
	if (bIsSyncingBlueprintModeState)
	{
		AdvanceMode = NewAdvanceMode;
		return;
	}

	if (AdvanceMode == NewAdvanceMode)
	{
		UpdateModeButtonFeedback();
		return;
	}

	AdvanceMode = NewAdvanceMode;

	SetBlueprintModeButtonActivated(AutoButtonPropertyName, AdvanceMode == EDialogAdvanceMode::Auto);
	SetBlueprintModeButtonActivated(SkipButtonPropertyName, AdvanceMode == EDialogAdvanceMode::Skip);
	UpdateModeButtonFeedback();

	if (const UWorld* World = GetWorld())
	{
		const float Now = World->GetTimeSeconds();
		NextAutoAdvanceTime = Now + AutoMinDelay;
		NextSkipAdvanceTime = Now + FMath::Max(0.01f, SkipInterval);
	}

	AdvanceModeChanged.Broadcast(AdvanceMode);
}

void USW_InGameUI::MarkDialogueAdvanceLocked(const float LockSeconds)
{
	if (const UWorld* World = GetWorld())
	{
		AdvanceLockedUntilTime = FMath::Max(AdvanceLockedUntilTime, World->GetTimeSeconds() + FMath::Max(0.0f, LockSeconds));
	}
}

bool USW_InGameUI::GetBacklogEntryAt(const int32 Index, FSW_BacklogEntry& OutEntry) const
{
	if (!BacklogEntries.IsValidIndex(Index))
	{
		return false;
	}

	OutEntry = BacklogEntries[Index];
	return true;
}

void USW_InGameUI::ClearBacklog()
{
	BacklogEntries.Reset();
	ClearLegacyBacklogBlueprintState(this);
	BacklogCleared.Broadcast();
}

void USW_InGameUI::PrepareLoadedGameState(const int32 LoadedRowDialog, UDataTable* LoadedDataTable, USoundBase* LoadedBackgroundMusic)
{
	ScriptManager = GetScriptManager();
	if (!ScriptManager)
	{
		return;
	}

	SetAdvanceMode(EDialogAdvanceMode::Manual);
	AdvanceLockedUntilTime = 0.0f;
	NextAutoAdvanceTime = 0.0f;
	NextSkipAdvanceTime = 0.0f;
	CurrentIndex = 0;

	ClearBacklog();
	LoadPersistentDialogState();

	if (LoadedDataTable)
	{
		ScriptManager->SetDataTable(LoadedDataTable);
		ScriptManager->CurrentChapter = ScriptManager->GetDataTableIndex(LoadedDataTable);
	}

	const int32 MaxDialogIndex = ScriptManager->GetMaxDialogIndex();
	ScriptManager->SetRowDialog(FMath::Clamp(LoadedRowDialog, 0, FMath::Max(0, MaxDialogIndex - 1)));
	ScriptManager->BackgroundMusic = LoadedBackgroundMusic;

	if (ScriptManager->AudioPlayer)
	{
		ScriptManager->AudioPlayer->Stop();
		if (LoadedBackgroundMusic)
		{
			ScriptManager->AudioPlayer->SetSound(LoadedBackgroundMusic);
			ScriptManager->AudioPlayer->Play();
		}
	}

	if (ScriptManager->ConversationalVoicePlayer)
	{
		ScriptManager->ConversationalVoicePlayer->Stop();
	}
}

bool USW_InGameUI::JumpToBacklogEntry(const FSW_BacklogEntry& Entry)
{
	ScriptManager = GetScriptManager();
	if (!ScriptManager || Entry.RowIndex < 0)
	{
		return false;
	}

	UDataTable* TargetDataTable = nullptr;
	if (Entry.ChapterIndex >= 0)
	{
		TargetDataTable = ScriptManager->GetDataTableByIndex(Entry.ChapterIndex);
	}

	if (!TargetDataTable && ScriptManager->DataTable)
	{
		TargetDataTable = ScriptManager->DataTable;
	}

	if (!TargetDataTable)
	{
		return false;
	}

	ScriptManager->SetDataTable(TargetDataTable);
	ScriptManager->CurrentChapter = ScriptManager->GetDataTableIndex(TargetDataTable);

	const int32 MaxDialogIndex = ScriptManager->GetMaxDialogIndex();
	if (Entry.RowIndex >= MaxDialogIndex)
	{
		return false;
	}

	SetAdvanceMode(EDialogAdvanceMode::Manual);
	AdvanceLockedUntilTime = 0.0f;
	ScriptManager->SetRowDialog(Entry.RowIndex);
	AdvanceDialogImmediately();

	if (UWidget* BacklogWidget = Cast<UWidget>(GetInGameUIObjectPropertyValue(this, TEXT("BP_BackLog"))))
	{
		BacklogWidget->RemoveFromParent();
	}

	if (ASW_PlayerController* PlayerController = Cast<ASW_PlayerController>(GetOwningPlayer()))
	{
		PlayerController->bIsLogExist = false;
	}

	ScriptManager->WidgetState = InGame;
	return true;
}

bool USW_InGameUI::JumpToBacklogIndex(const int32 Index)
{
	FSW_BacklogEntry Entry;
	return GetBacklogEntryAt(Index, Entry) && JumpToBacklogEntry(Entry);
}

bool USW_InGameUI::TryStartChapterSwitch()
{
	ScriptManager = GetScriptManager();
	if (!ScriptManager)
	{
		return false;
	}

	const int32 MaxDialogIndex = ScriptManager->GetMaxDialogIndex();
	if (ScriptManager->rowDialog < MaxDialogIndex)
	{
		return false;
	}

	ScriptManager->CurrentChapter = ScriptManager->GetDataTableIndex(ScriptManager->DataTable);
	if (ScriptManager->CurrentChapter < 0)
	{
		SetAdvanceMode(EDialogAdvanceMode::Manual);
		return true;
	}

	UDataTable* NextChapter = ScriptManager->GetDataTableByIndex(ScriptManager->CurrentChapter + 1);
	if (!NextChapter)
	{
		SetAdvanceMode(EDialogAdvanceMode::Manual);
		return true;
	}

	ScriptManager->rowDialog = 0;
	ScriptManager->CurrentChapter++;
	ScriptManager->DataTable = NextChapter;
	MarkDialogueAdvanceLocked(ChapterSwitchAdvanceLockSeconds);
	SetAdvanceMode(EDialogAdvanceMode::Manual);
	SwitchChapterCG.Broadcast();
	return true;
}

bool USW_InGameUI::CanAdvanceDialog() const
{
	if (!ScriptManager || ScriptManager->WidgetState != InGame)
	{
		return false;
	}

	const UWorld* World = GetWorld();
	return !World || World->GetTimeSeconds() >= AdvanceLockedUntilTime;
}

bool USW_InGameUI::IsAdvanceAnimationPlaying() const
{
	return bIsPlayAnimation || GetInGameUIBoolPropertyValue(this, TEXT("IsPlayAnimation"));
}

void USW_InGameUI::SyncAdvanceModeFromBlueprintButtons()
{
	if (bIsSyncingBlueprintModeState)
	{
		return;
	}

	const bool bAutoActivated = IsBlueprintModeButtonActivated(AutoButtonPropertyName);
	const bool bSkipActivated = IsBlueprintModeButtonActivated(SkipButtonPropertyName);
	const EDialogAdvanceMode SyncedMode = bSkipActivated ? EDialogAdvanceMode::Skip : (bAutoActivated ? EDialogAdvanceMode::Auto : EDialogAdvanceMode::Manual);

	if (SyncedMode == AdvanceMode)
	{
		UpdateModeButtonFeedback();
		return;
	}

	bIsSyncingBlueprintModeState = true;
	AdvanceMode = SyncedMode;
	bIsSyncingBlueprintModeState = false;

	if (AdvanceMode == EDialogAdvanceMode::Skip)
	{
		SetBlueprintModeButtonActivated(AutoButtonPropertyName, false);
	}

	if (const UWorld* World = GetWorld())
	{
		const float Now = World->GetTimeSeconds();
		NextAutoAdvanceTime = Now + AutoMinDelay;
		NextSkipAdvanceTime = Now + FMath::Max(0.01f, SkipInterval);
	}

	UpdateModeButtonFeedback();
	AdvanceModeChanged.Broadcast(AdvanceMode);
}

bool USW_InGameUI::IsBlueprintModeButtonActivated(const FName ButtonName) const
{
	UObject* ButtonContainer = GetInGameUIObjectPropertyValue(const_cast<USW_InGameUI*>(this), ButtonContainerPropertyName);
	UObject* ButtonWidget = GetInGameUIObjectPropertyValue(ButtonContainer, ButtonName);
	return GetInGameUIBoolPropertyValue(ButtonWidget, TEXT("bIsActivated"));
}

void USW_InGameUI::SetBlueprintModeButtonActivated(const FName ButtonName, const bool bActivated)
{
	UObject* ButtonContainer = GetInGameUIObjectPropertyValue(this, ButtonContainerPropertyName);
	UObject* ButtonWidget = GetInGameUIObjectPropertyValue(ButtonContainer, ButtonName);
	SetInGameUIBoolPropertyValue(ButtonWidget, TEXT("bIsActivated"), bActivated);

	if (UWidget* Widget = Cast<UWidget>(ButtonWidget))
	{
		Widget->SetRenderOpacity(bActivated ? 1.0f : 0.55f);
	}
}

void USW_InGameUI::UpdateModeButtonFeedback() const
{
	UObject* ButtonContainer = GetInGameUIObjectPropertyValue(const_cast<USW_InGameUI*>(this), ButtonContainerPropertyName);
	UObject* AutoButton = GetInGameUIObjectPropertyValue(ButtonContainer, AutoButtonPropertyName);
	UObject* SkipButton = GetInGameUIObjectPropertyValue(ButtonContainer, SkipButtonPropertyName);

	if (UWidget* AutoWidget = Cast<UWidget>(AutoButton))
	{
		AutoWidget->SetRenderOpacity(AdvanceMode == EDialogAdvanceMode::Auto ? 1.0f : 0.55f);
	}

	if (UWidget* SkipWidget = Cast<UWidget>(SkipButton))
	{
		SkipWidget->SetRenderOpacity(AdvanceMode == EDialogAdvanceMode::Skip ? 1.0f : 0.55f);
	}
}

void USW_InGameUI::ConfigureLegacyBacklogEntryWidgetJumps()
{
	if (!ScriptManager || ScriptManager->WidgetState != BackLog)
	{
		return;
	}

	const FArrayProperty* ArrayProperty = FindFProperty<FArrayProperty>(GetClass(), TEXT("BP_NameAndDialogueArray"));
	if (!ArrayProperty)
	{
		return;
	}

	const FObjectPropertyBase* ObjectProperty = CastField<FObjectPropertyBase>(ArrayProperty->Inner);
	if (!ObjectProperty)
	{
		return;
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(this));
	for (int32 Index = 0; Index < ArrayHelper.Num(); ++Index)
	{
		if (USW_UIBase* EntryWidget = Cast<USW_UIBase>(ObjectProperty->GetObjectPropertyValue(ArrayHelper.GetRawPtr(Index))))
		{
			EntryWidget->bJumpToBacklogOnMouseDown = true;
			EntryWidget->BacklogIndex = Index;
		}
	}
}

void USW_InGameUI::LoadPersistentDialogState()
{
	ReadDialogKeys.Reset();

	const ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode)
	{
		return;
	}

	const USW_SystemData* SystemData = GameMode->GetCGSlotData();
	if (SystemData)
	{
		ReadDialogKeys = SystemData->ReadDialogKeys;
	}
}

FName USW_InGameUI::GetCurrentDialogReadKey() const
{
	if (!ScriptManager || !ScriptManager->DataTable)
	{
		return NAME_None;
	}

	const TArray<FName> RowNames = ScriptManager->DataTable->GetRowNames();
	if (!RowNames.IsValidIndex(ScriptManager->rowDialog))
	{
		return NAME_None;
	}

	return FName(*FString::Printf(TEXT("%s:%s"), *ScriptManager->DataTable->GetPathName(), *RowNames[ScriptManager->rowDialog].ToString()));
}

bool USW_InGameUI::IsCurrentDialogRead() const
{
	const FName ReadKey = GetCurrentDialogReadKey();
	if (ReadKey.IsNone())
	{
		return false;
	}

	if (ReadDialogKeys.Contains(ReadKey))
	{
		return true;
	}

	const ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this));
	return GameMode && GameMode->IsDialogKeyRead(ReadKey);
}

FName USW_InGameUI::MarkCurrentDialogRead()
{
	const FName ReadKey = GetCurrentDialogReadKey();
	if (ReadKey.IsNone())
	{
		return NAME_None;
	}

	if (!ReadDialogKeys.Contains(ReadKey))
	{
		ReadDialogKeys.Add(ReadKey);

		if (const ASW_GameMode* GameMode = Cast<ASW_GameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->AddReadDialogKey(ReadKey);
		}
	}

	return ReadKey;
}

bool USW_InGameUI::AddBacklogEntry(const FDialogStruct& InDialogStruct, const FName DialogKey)
{
	if (!DialogKey.IsNone())
	{
		const bool bAlreadyRecorded = BacklogEntries.ContainsByPredicate(
			[DialogKey](const FSW_BacklogEntry& ExistingEntry)
			{
				return ExistingEntry.DialogKey == DialogKey;
			});

		if (bAlreadyRecorded)
		{
			return false;
		}
	}

	FSW_BacklogEntry Entry;
	Entry.SpeakerName = InDialogStruct.Name;
	Entry.Dialogue = InDialogStruct.Dialog;
	Entry.DialogKey = DialogKey;
	Entry.RowIndex = ScriptManager ? ScriptManager->rowDialog : INDEX_NONE;
	Entry.ChapterIndex = ScriptManager ? ScriptManager->GetDataTableIndex(ScriptManager->DataTable) : INDEX_NONE;

	BacklogEntries.Add(Entry);

	if (MaxBacklogEntries > 0 && BacklogEntries.Num() > MaxBacklogEntries)
	{
		BacklogEntries.RemoveAt(0, BacklogEntries.Num() - MaxBacklogEntries, false);
	}

	BacklogEntryRecorded.Broadcast(Entry);
	return true;
}

float USW_InGameUI::CalculateAutoDelay(const FDialogStruct& InDialogStruct) const
{
	const int32 CharacterCount = InDialogStruct.Dialog.ToString().Len();
	const float TextRevealDuration = ScriptManager ? ScriptManager->TextDisplaySpeed * CharacterCount : 0.0f;
	const float TextDelay = AutoBaseDelay + TextRevealDuration + CharacterCount * AutoPerCharacterDelay;

	float VoiceDelay = 0.0f;
	if (InDialogStruct.ConversationalVoice)
	{
		const float VoiceDuration = InDialogStruct.ConversationalVoice->GetDuration();
		if (VoiceDuration > 0.0f && VoiceDuration < 3600.0f)
		{
			VoiceDelay = VoiceDuration + AutoVoiceTailDelay;
		}
	}

	return FMath::Clamp(FMath::Max(TextDelay, VoiceDelay), AutoMinDelay, AutoMaxDelay);
}

void USW_InGameUI::RefreshAutoAdvanceTime(const FDialogStruct& InDialogStruct)
{
	if (const UWorld* World = GetWorld())
	{
		NextAutoAdvanceTime = World->GetTimeSeconds() + CalculateAutoDelay(InDialogStruct);
	}
}

void USW_InGameUI::UpdateStruct()
{
	if (!ScriptManager)
	{
		DialogStruct = nullptr;
		PreviousDialogStruct = nullptr;
		return;
	}

	DialogStruct = ScriptManager->GetDialogStruct();
	PreviousDialogStruct = ScriptManager->GetPreviousDialogStruct();
	if (!PreviousDialogStruct)
	{
		PreviousDialogStruct = DialogStruct;
	}
}

void USW_InGameUI::SetName(FDialogStruct* dialogRow)
{
	if (!dialogRow)
	{
		return;
	}

	if (dialogRow->Name.IsEmpty())
	{
		NameBoxBackground->SetBrushColor(FColor(0.0f, 0.0f, 0.0f, 0.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
	else
	{
		NameBoxBackground->SetBrushColor(FColor(255.0f, 255.0f, 255.0f, 255.0f));
		NameBoxBackground->SetContentColorAndOpacity(FColor(255.0f, 255.0f, 255.0f, 255.0f));
		TextBlock_Name->SetText(dialogRow->Name);
	}
}

void USW_InGameUI::SetBackground(FDialogStruct* dialogRow)
{
	if (dialogRow)
	{
		TEX_Background->SetBrushFromTexture(dialogRow->Background, false);
	}
}

void USW_InGameUI::SetMusic(FDialogStruct* dialogRow)
{
	if (dialogRow && dialogRow->BackgroundSound != nullptr)
	{
		ScriptManager->BackgroundMusic = dialogRow->BackgroundSound;
		ScriptManager->AudioPlayer->Stop();
		ScriptManager->AudioPlayer->SetSound(dialogRow->BackgroundSound);
		ScriptManager->AudioPlayer->Play();
	}
}

void USW_InGameUI::SetConversationalVoice(FDialogStruct* dialogRow)
{
	if (dialogRow && dialogRow->ConversationalVoice != nullptr)
	{
		if (ScriptManager->ConversationalVoicePlayer != nullptr)
		{
			ScriptManager->ConversationalVoicePlayer->Stop();
		}
		ScriptManager->ConversationalVoicePlayer->SetSound(dialogRow->ConversationalVoice);
		ScriptManager->ConversationalVoicePlayer->Play();
	}
}

void USW_InGameUI::SwitchChapter()
{
	TryStartChapterSwitch();
}
