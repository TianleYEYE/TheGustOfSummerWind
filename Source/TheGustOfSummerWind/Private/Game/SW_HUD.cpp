// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_HUD.h"

#include "Engine/DataTable.h"
#include "Game/SW_ScriptManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UI/InGameUI/SW_InGameUI.h"
#include "UI/LoadingUI/SW_LoadingUI.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogSWUITransition, Log, All);

ASW_HUD::ASW_HUD()
{
}

namespace
{
const TCHAR* ToString(const ESWPendingInGameTransition TransitionType)
{
	switch (TransitionType)
	{
	case ESWPendingInGameTransition::NewGame:
		return TEXT("NewGame");
	case ESWPendingInGameTransition::LoadFromMenu:
		return TEXT("LoadFromMenu");
	case ESWPendingInGameTransition::LoadFromInGame:
		return TEXT("LoadFromInGame");
	default:
		return TEXT("None");
	}
}

const TCHAR* ToString(const ESWTransitionPhase TransitionPhase)
{
	switch (TransitionPhase)
	{
	case ESWTransitionPhase::FadingOutContinue:
		return TEXT("FadingOutContinue");
	case ESWTransitionPhase::FadingOutMenu:
		return TEXT("FadingOutMenu");
	case ESWTransitionPhase::FadingInLoading:
		return TEXT("FadingInLoading");
	case ESWTransitionPhase::FadingOutLoading:
		return TEXT("FadingOutLoading");
	case ESWTransitionPhase::FadingInInGame:
		return TEXT("FadingInInGame");
	default:
		return TEXT("None");
	}
}
}


void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeViewModels();
	CreateMenu();
	BindMenuButtons();

	if (SystemViewModel)
	{
		SystemViewModel->LoadData();
	}
}

void ASW_HUD::InitializeViewModels()
{
	if (!LoadScreenViewModelClass)
	{
		LoadScreenViewModelClass = LoadClass<UMVVM_LoadScreen>(nullptr, TEXT("/Game/Blueprints/UI/ViewModel/BP_LoadScreenViewModel.BP_LoadScreenViewModel_C"));
	}

	if (!SystemViewModelClass)
	{
		SystemViewModelClass = LoadClass<UMVVM_System>(nullptr, TEXT("/Game/Blueprints/UI/ViewModel/BP_SystemViewModel.BP_SystemViewModel_C"));
	}

	if (LoadScreenViewModelClass)
	{
		LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
		if (LoadScreenViewModel)
		{
			LoadScreenViewModel->InitializeLoadSlots();
			LoadScreenViewModel->LoadData();
		}
	}

	if (SystemViewModelClass)
	{
		SystemViewModel = NewObject<UMVVM_System>(this, SystemViewModelClass);
		if (SystemViewModel)
		{
			SystemViewModel->InitializeLoadSlots();
		}
	}
}

void ASW_HUD::CreateMenu()
{
	if (!MenuClass)
	{
		MenuClass = LoadClass<USW_InMenuUI>(nullptr, TEXT("/Game/Blueprints/UI/MenuUI/BP_MenuUI.BP_MenuUI_C"));
	}

	if (!MenuClass || !GetOwningPlayerController())
	{
		return;
	}

	Menu = CreateWidget<USW_InMenuUI>(GetOwningPlayerController(), MenuClass);
	if (Menu)
	{
		BindFadeCallback(Menu);
		Menu->AddToViewport();
	}
}

void ASW_HUD::BindMenuButtons()
{
	if (!Menu || !Menu->BP_ButtonOfInMenu)
	{
		return;
	}

	if (Menu->BP_ButtonOfInMenu->BP_NewGame)
	{
		Menu->BP_ButtonOfInMenu->BP_NewGame->NewGame.AddUObject(this, &ASW_HUD::HandleNewGameTransition);
	}

	if (Menu->BP_ButtonOfInMenu->BP_Continue)
	{
		Menu->BP_ButtonOfInMenu->BP_Continue->Continue.AddUniqueDynamic(this, &ASW_HUD::HandleContinueTransition);
	}

	if (Menu->BP_ButtonOfInMenu->BP_Album)
	{
		Menu->BP_ButtonOfInMenu->BP_Album->Album.AddUObject(this, &ASW_HUD::HandleAlbumTransition);
	}

	if (Menu->BP_ButtonOfInMenu->BP_Setting)
	{
		Menu->BP_ButtonOfInMenu->BP_Setting->Setting.AddUObject(this, &ASW_HUD::HandleSettingTransition);
	}
}

void ASW_HUD::HandleNewGameTransition()
{
	ASW_ScriptManager* ScriptManager = GetScriptManager();
	if (ScriptManager && !ScriptManager->DataTable)
	{
		ScriptManager->SetDataTable(ScriptManager->GetDataTableByIndex(0));
	}

	StartInGameTransition(
		ESWPendingInGameTransition::NewGame,
		0,
		ScriptManager ? ScriptManager->GetDataTable() : nullptr,
		nullptr);
}

void ASW_HUD::HandleContinueTransition()
{
	OpenContinueUI(ESWSaveLoadMode::LoadFromMenu);
}

void ASW_HUD::HandleAlbumTransition()
{
	if (!AlbumUIClass)
	{
		AlbumUIClass = LoadClass<USW_UIBase>(nullptr, TEXT("/Game/Blueprints/UI/AlbumUI/BP_AlbumUI.BP_AlbumUI_C"));
	}

	if (SystemViewModel)
	{
		SystemViewModel->LoadData();
	}

	SetOverlayWidget(AlbumUI, AlbumUIClass, ::AlbumUI, 2);
}

void ASW_HUD::HandleSettingTransition()
{
	if (!SettingUIClass)
	{
		SettingUIClass = LoadClass<USW_UIBase>(nullptr, TEXT("/Game/Blueprints/UI/SettingUI/BP_SettingUI.BP_SettingUI_C"));
	}

	SetOverlayWidget(SettingUI, SettingUIClass, ::SettingUI, 2);
}

void ASW_HUD::HandleOverlayFadeComplete(bool bIsPlayingAnimation)
{
	UE_LOG(LogSWUITransition, Verbose, TEXT("Received fade callback: IsPlaying=%s, Phase=%s"),
		bIsPlayingAnimation ? TEXT("true") : TEXT("false"),
		ToString(TransitionPhase));

	if (bIsPlayingAnimation)
	{
		return;
	}

	switch (TransitionPhase)
	{
	case ESWTransitionPhase::FadingOutContinue:
		ContinuePendingTransition();
		return;

	case ESWTransitionPhase::FadingOutMenu:
		BeginLoadingFadeIn();
		return;

	case ESWTransitionPhase::FadingInLoading:
		PreparePendingInGameState();
		BeginLoadingFadeOut();
		return;

	case ESWTransitionPhase::FadingOutLoading:
		BeginInGameFadeIn();
		return;

	case ESWTransitionPhase::FadingInInGame:
		CompleteInGameTransition();
		return;

	default:
		break;
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		if (ContinueUI && ContinueUI->IsInViewport())
		{
			UE_LOG(LogSWUITransition, Verbose, TEXT("Overlay fade complete; ContinueUI still visible, restoring save/load widget state."));
			ScriptManager->WidgetState = ContinueUIMode == ESWSaveLoadMode::LoadFromMenu ? SaveOrLoad : InGameSaveOrLoad;
			return;
		}

		UE_LOG(LogSWUITransition, Verbose, TEXT("Overlay fade complete; restoring widget state to %s."),
			InGameUI && InGameUI->IsInViewport() ? TEXT("InGame") : TEXT("Menu"));
		ScriptManager->WidgetState = InGameUI && InGameUI->IsInViewport() ? InGame : ::Menu;
	}
}

void ASW_HUD::OpenContinueUI(const ESWSaveLoadMode InMode)
{
	if (!ContinueUIClass)
	{
		ContinueUIClass = LoadClass<USW_UIBase>(nullptr, TEXT("/Game/Blueprints/UI/ContinueUI/BP_ContinueUI.BP_ContinueUI_C"));
	}

	ContinueUIMode = InMode;
	PendingContinueSlotIndex = INDEX_NONE;

	if (LoadScreenViewModel)
	{
		LoadScreenViewModel->LoadData();
	}

	SetOverlayWidget(ContinueUI, ContinueUIClass, InMode == ESWSaveLoadMode::LoadFromMenu ? SaveOrLoad : InGameSaveOrLoad, 2);
	RefreshContinueUIState();
}

void ASW_HUD::CloseContinueUI(const bool bRestoreUnderlyingState)
{
	PendingContinueSlotIndex = INDEX_NONE;

	if (ContinueUI)
	{
		ContinueUI->RemoveFromParent();
	}

	if (!bRestoreUnderlyingState)
	{
		return;
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		ScriptManager->WidgetState = InGameUI && InGameUI->IsInViewport() ? InGame : ::Menu;
	}
}

bool ASW_HUD::SaveCurrentGameToSlot(const int32 SlotIndex)
{
	if (!LoadScreenViewModel)
	{
		return false;
	}

	ASW_ScriptManager* ScriptManager = GetScriptManager();
	if (!ScriptManager || !ScriptManager->GetDataTable())
	{
		return false;
	}

	LoadScreenViewModel->SaveSlotButtonPressed(
		SlotIndex,
		ScriptManager->GetDataTable(),
		ScriptManager->GetRowDialog(),
		ScriptManager->BackgroundMusic,
		FDateTime::Now());

	RefreshContinueUIState();
	return true;
}

bool ASW_HUD::LoadGameFromSlot(const int32 SlotIndex)
{
	if (!LoadScreenViewModel)
	{
		return false;
	}

	UMVVM_LoadSlot* LoadSlotViewModel = LoadScreenViewModel->GetLoadSlotViewModelByIndex(SlotIndex);
	if (!LoadSlotViewModel || LoadSlotViewModel->SlotStatus != Load || !LoadSlotViewModel->GetDataTable())
	{
		return false;
	}

	StartInGameTransition(
		ContinueUIMode == ESWSaveLoadMode::LoadFromMenu ? ESWPendingInGameTransition::LoadFromMenu : ESWPendingInGameTransition::LoadFromInGame,
		LoadSlotViewModel->GetRowDialog(),
		LoadSlotViewModel->GetDataTable(),
		LoadSlotViewModel->GetBackgroundMusic());
	return true;
}

void ASW_HUD::SetOverlayWidget(TObjectPtr<USW_UIBase>& Storage, TSubclassOf<USW_UIBase> WidgetClass, const EWidgetStatus NewWidgetState, const int32 ZOrder)
{
	if (!Storage && WidgetClass && GetOwningPlayerController())
	{
		Storage = CreateWidget<USW_UIBase>(GetOwningPlayerController(), WidgetClass);
		if (Storage)
		{
			BindFadeCallback(Storage);
		}
	}

	if (!Storage)
	{
		return;
	}

	if (!Storage->IsInViewport())
	{
		Storage->AddToViewport(ZOrder);
		Storage->PlayEnterFadeAnimation();
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		ScriptManager->WidgetState = NewWidgetState;
	}
}

void ASW_HUD::EnsureInGameUI()
{
	if (!InGameUIClass)
	{
		InGameUIClass = LoadClass<USW_InGameUI>(nullptr, TEXT("/Game/Blueprints/UI/InGameUI/BP_InGameUI.BP_InGameUI_C"));
	}

	if (!InGameUI && InGameUIClass && GetOwningPlayerController())
	{
		InGameUI = CreateWidget<USW_InGameUI>(GetOwningPlayerController(), InGameUIClass);
		BindFadeCallback(InGameUI);
	}
}

void ASW_HUD::EnsureLoadingUI()
{
	if (!LoadingUIClass)
	{
		LoadingUIClass = LoadClass<USW_LoadingUI>(nullptr, TEXT("/Game/Blueprints/UI/LoadingUI/BP_LoadingUI.BP_LoadingUI_C"));
		if (!LoadingUIClass)
		{
			UE_LOG(LogSWUITransition, Warning, TEXT("Failed to load BP_LoadingUI. Falling back to native USW_LoadingUI."));
			LoadingUIClass = USW_LoadingUI::StaticClass();
		}
	}

	if (!LoadingUI && LoadingUIClass && GetOwningPlayerController())
	{
		LoadingUI = CreateWidget<USW_LoadingUI>(GetOwningPlayerController(), LoadingUIClass);
		UE_LOG(LogSWUITransition, Log, TEXT("Created LoadingUI widget: %s"), *GetNameSafe(LoadingUI));
		BindFadeCallback(LoadingUI);
	}
}

void ASW_HUD::RefreshContinueUIState() const
{
	if (ContinueUI)
	{
		ContinueUI->RefreshSaveLoadUiState();
	}
}

ASW_ScriptManager* ASW_HUD::GetScriptManager() const
{
	return Cast<ASW_ScriptManager>(UGameplayStatics::GetActorOfClass(this, ASW_ScriptManager::StaticClass()));
}

void ASW_HUD::BindFadeCallback(USW_UIBase* Widget)
{
	if (Widget)
	{
		UE_LOG(LogSWUITransition, Verbose, TEXT("Binding fade callback for widget: %s"), *GetNameSafe(Widget));
		Widget->PlayingWidgetFade.AddUniqueDynamic(this, &ASW_HUD::HandleOverlayFadeComplete);
	}
}

void ASW_HUD::StartInGameTransition(const ESWPendingInGameTransition InTransitionType, const int32 InRowDialog, UDataTable* InDataTable, USoundBase* InBackgroundMusic)
{
	if (PendingInGameTransition != ESWPendingInGameTransition::None)
	{
		UE_LOG(LogSWUITransition, Warning, TEXT("Ignored transition request %s because another transition is already pending: %s."),
			ToString(InTransitionType),
			ToString(PendingInGameTransition));
		return;
	}

	PendingInGameTransition = InTransitionType;
	PendingRowDialog = FMath::Max(0, InRowDialog);
	PendingDataTable = InDataTable;
	PendingBackgroundMusic = InBackgroundMusic;
	PendingContinueSlotIndex = INDEX_NONE;

	UE_LOG(LogSWUITransition, Log, TEXT("Starting in-game transition: Type=%s, Row=%d, DataTable=%s, BGM=%s"),
		ToString(PendingInGameTransition),
		PendingRowDialog,
		*GetNameSafe(PendingDataTable),
		*GetNameSafe(PendingBackgroundMusic));

	if (ContinueUI && ContinueUI->IsInViewport())
	{
		TransitionPhase = ESWTransitionPhase::FadingOutContinue;
		UE_LOG(LogSWUITransition, Log, TEXT("Transition phase -> %s"), ToString(TransitionPhase));
		ContinueUI->PlayWidgetFade.Broadcast(true);
		return;
	}

	if (Menu && Menu->IsInViewport())
	{
		TransitionPhase = ESWTransitionPhase::FadingOutMenu;
		UE_LOG(LogSWUITransition, Log, TEXT("Transition phase -> %s"), ToString(TransitionPhase));
		Menu->PlayWidgetFade.Broadcast(true);
		return;
	}

	UE_LOG(LogSWUITransition, Verbose, TEXT("No ContinueUI/Menu fade-out required; proceeding directly to Loading fade-in."));
	BeginLoadingFadeIn();
}

void ASW_HUD::ContinuePendingTransition()
{
	if (PendingInGameTransition == ESWPendingInGameTransition::None)
	{
		UE_LOG(LogSWUITransition, Warning, TEXT("ContinuePendingTransition called without a pending in-game transition."));
		TransitionPhase = ESWTransitionPhase::None;
		return;
	}

	if (Menu && Menu->IsInViewport())
	{
		TransitionPhase = ESWTransitionPhase::FadingOutMenu;
		UE_LOG(LogSWUITransition, Log, TEXT("Transition phase -> %s"), ToString(TransitionPhase));
		Menu->PlayWidgetFade.Broadcast(true);
		return;
	}

	UE_LOG(LogSWUITransition, Verbose, TEXT("ContinueUI fade-out complete; proceeding directly to Loading fade-in."));
	BeginLoadingFadeIn();
}

void ASW_HUD::BeginLoadingFadeIn()
{
	EnsureLoadingUI();
	if (!LoadingUI)
	{
		UE_LOG(LogSWUITransition, Warning, TEXT("LoadingUI unavailable; preparing in-game state immediately."));
		PreparePendingInGameState();
		BeginInGameFadeIn();
		return;
	}

	if (!LoadingUI->IsInViewport())
	{
		UE_LOG(LogSWUITransition, Log, TEXT("Adding LoadingUI to viewport."));
		LoadingUI->AddToViewport(100);
	}

	TransitionPhase = ESWTransitionPhase::FadingInLoading;
	UE_LOG(LogSWUITransition, Log, TEXT("Transition phase -> %s"), ToString(TransitionPhase));
	LoadingUI->PlayEnterFadeAnimationAndNotify();
}

void ASW_HUD::BeginLoadingFadeOut()
{
	if (!LoadingUI || !LoadingUI->IsInViewport())
	{
		UE_LOG(LogSWUITransition, Warning, TEXT("LoadingUI is not visible during fade-out request; proceeding to InGame fade-in."));
		BeginInGameFadeIn();
		return;
	}

	TransitionPhase = ESWTransitionPhase::FadingOutLoading;
	UE_LOG(LogSWUITransition, Log, TEXT("Transition phase -> %s"), ToString(TransitionPhase));
	LoadingUI->PlayWidgetFade.Broadcast(true);
}

void ASW_HUD::PreparePendingInGameState()
{
	EnsureInGameUI();
	if (!InGameUI)
	{
		UE_LOG(LogSWUITransition, Error, TEXT("Failed to prepare pending in-game state because InGameUI could not be created."));
		ClearPendingTransition();
		return;
	}

	ASW_ScriptManager* ScriptManager = GetScriptManager();
	if (ScriptManager)
	{
		if (!PendingDataTable && !ScriptManager->DataTable)
		{
			ScriptManager->SetDataTable(ScriptManager->GetDataTableByIndex(0));
		}

		ScriptManager->WidgetState = InGame;
	}

	UDataTable* TargetDataTable = PendingDataTable ? PendingDataTable.Get() : (ScriptManager ? ScriptManager->GetDataTable() : nullptr);
	UE_LOG(LogSWUITransition, Log, TEXT("Preparing in-game state: Row=%d, DataTable=%s, BGM=%s"),
		PendingRowDialog,
		*GetNameSafe(TargetDataTable),
		*GetNameSafe(PendingBackgroundMusic));
	InGameUI->PrepareLoadedGameState(PendingRowDialog, TargetDataTable, PendingBackgroundMusic);

	if (!InGameUI->IsInViewport())
	{
		UE_LOG(LogSWUITransition, Log, TEXT("Adding InGameUI to viewport in hidden state before fade-in."));
		InGameUI->AddToViewport(1);
	}

	InGameUI->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogSWUITransition, Verbose, TEXT("InGameUI hidden until fade-in begins."));
}

void ASW_HUD::BeginInGameFadeIn()
{
	if (!InGameUI)
	{
		UE_LOG(LogSWUITransition, Verbose, TEXT("InGameUI missing at fade-in step; preparing pending in-game state now."));
		PreparePendingInGameState();
	}

	if (!InGameUI)
	{
		UE_LOG(LogSWUITransition, Error, TEXT("Failed to begin in-game fade-in because InGameUI is still null."));
		ClearPendingTransition();
		return;
	}

	InGameUI->SetVisibility(ESlateVisibility::Visible);
	TransitionPhase = ESWTransitionPhase::FadingInInGame;
	UE_LOG(LogSWUITransition, Log, TEXT("Transition phase -> %s"), ToString(TransitionPhase));
	InGameUI->PlayEnterFadeAnimationAndNotify();
}

void ASW_HUD::CompleteInGameTransition()
{
	if (InGameUI)
	{
		UE_LOG(LogSWUITransition, Log, TEXT("In-game fade-in complete; advancing first dialog line."));
		InGameUI->RequestAdvanceDialog(false);
	}

	UE_LOG(LogSWUITransition, Log, TEXT("Transition completed successfully."));
	ClearPendingTransition();
}

void ASW_HUD::ClearPendingTransition()
{
	UE_LOG(LogSWUITransition, Verbose, TEXT("Clearing pending transition. PreviousType=%s, PreviousPhase=%s, Row=%d"),
		ToString(PendingInGameTransition),
		ToString(TransitionPhase),
		PendingRowDialog);

	TransitionPhase = ESWTransitionPhase::None;
	PendingInGameTransition = ESWPendingInGameTransition::None;
	PendingRowDialog = 0;
	PendingDataTable = nullptr;
	PendingBackgroundMusic = nullptr;
	PendingContinueSlotIndex = INDEX_NONE;
}
