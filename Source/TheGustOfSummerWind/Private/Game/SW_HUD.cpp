// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_HUD.h"

#include "Game/SW_ScriptManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameUI/SW_InGameUI.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

ASW_HUD::ASW_HUD()
{
}


void ASW_HUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeViewModels();
	CreateMenu();
	BindMenuTransitions();

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
		Menu->AddToViewport();
	}
}

void ASW_HUD::BindMenuTransitions()
{
	if (!Menu)
	{
		return;
	}

	Menu->OnNewGameTransition.AddUniqueDynamic(this, &ASW_HUD::HandleNewGameTransition);
	Menu->OnContinueTransition.AddUniqueDynamic(this, &ASW_HUD::HandleContinueTransition);
	Menu->OnAlbumTransition.AddUniqueDynamic(this, &ASW_HUD::HandleAlbumTransition);
	Menu->OnSettingTransition.AddUniqueDynamic(this, &ASW_HUD::HandleSettingTransition);
}

void ASW_HUD::HandleNewGameTransition()
{
	if (!InGameUIClass)
	{
		InGameUIClass = LoadClass<USW_InGameUI>(nullptr, TEXT("/Game/Blueprints/UI/InGameUI/BP_InGameUI.BP_InGameUI_C"));
	}

	if (!InGameUI && InGameUIClass && GetOwningPlayerController())
	{
		InGameUI = CreateWidget<USW_InGameUI>(GetOwningPlayerController(), InGameUIClass);
	}

	if (!InGameUI)
	{
		return;
	}

	if (!InGameUI->IsInViewport())
	{
		InGameUI->AddToViewport(1);
	}

	if (Menu)
	{
		Menu->RemoveFromParent();
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		if (!ScriptManager->DataTable)
		{
			ScriptManager->SetDataTable(ScriptManager->GetDataTableByIndex(0));
		}

		ScriptManager->SetRowDialog(0);
		ScriptManager->WidgetState = InGame;
	}

	InGameUI->PrepareLoadedGameState(0, GetScriptManager() ? GetScriptManager()->DataTable : nullptr, nullptr);
	InGameUI->RequestAdvanceDialog(false);
}

void ASW_HUD::HandleContinueTransition()
{
	if (!ContinueUIClass)
	{
		ContinueUIClass = LoadClass<USW_UIBase>(nullptr, TEXT("/Game/Blueprints/UI/ContinueUI/BP_ContinueUI.BP_ContinueUI_C"));
	}

	if (LoadScreenViewModel)
	{
		LoadScreenViewModel->LoadData();
	}

	SetOverlayWidget(ContinueUI, ContinueUIClass, SaveOrLoad, 2);
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
	if (bIsPlayingAnimation)
	{
		return;
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		ScriptManager->WidgetState = InGameUI && InGameUI->IsInViewport() ? InGame : ::Menu;
	}
}

void ASW_HUD::SetOverlayWidget(TObjectPtr<USW_UIBase>& Storage, TSubclassOf<USW_UIBase> WidgetClass, const EWidgetStatus NewWidgetState, const int32 ZOrder)
{
	if (!Storage && WidgetClass && GetOwningPlayerController())
	{
		Storage = CreateWidget<USW_UIBase>(GetOwningPlayerController(), WidgetClass);
		if (Storage)
		{
			Storage->PlayingWidgetFade.AddUniqueDynamic(this, &ASW_HUD::HandleOverlayFadeComplete);
		}
	}

	if (!Storage)
	{
		return;
	}

	if (!Storage->IsInViewport())
	{
		Storage->AddToViewport(ZOrder);
	}

	if (ASW_ScriptManager* ScriptManager = GetScriptManager())
	{
		ScriptManager->WidgetState = NewWidgetState;
	}
}

ASW_ScriptManager* ASW_HUD::GetScriptManager() const
{
	return Cast<ASW_ScriptManager>(UGameplayStatics::GetActorOfClass(this, ASW_ScriptManager::StaticClass()));
}
