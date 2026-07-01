// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SW_UIBase.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ComboBoxString.h"
#include "Components/PanelWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Game/SW_SaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Texture2D.h"
#include "Game/SW_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/InGameUI/SW_InGameUI.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "UI/ViewModel/MVVM_SettingSlot.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_System.h"
#include "UObject/UnrealType.h"

namespace
{
UWidgetAnimation* FindWidgetAnimationByName(const UUserWidget* Widget, const TArray<FName>& CandidateNames)
{
	if (!Widget)
	{
		return nullptr;
	}

	for (const FName& CandidateName : CandidateNames)
	{
		const FObjectPropertyBase* AnimationProperty = FindFProperty<FObjectPropertyBase>(Widget->GetClass(), CandidateName);
		if (AnimationProperty && AnimationProperty->PropertyClass->IsChildOf(UWidgetAnimation::StaticClass()))
		{
			if (UWidgetAnimation* Animation = Cast<UWidgetAnimation>(AnimationProperty->GetObjectPropertyValue_InContainer(Widget)))
			{
				return Animation;
			}
		}
	}

	for (TFieldIterator<FObjectPropertyBase> PropertyIt(Widget->GetClass()); PropertyIt; ++PropertyIt)
	{
		const FObjectPropertyBase* ObjectProperty = *PropertyIt;
		if (ObjectProperty && ObjectProperty->PropertyClass->IsChildOf(UWidgetAnimation::StaticClass()))
		{
			if (UWidgetAnimation* Animation = Cast<UWidgetAnimation>(ObjectProperty->GetObjectPropertyValue_InContainer(Widget)))
			{
				return Animation;
			}
		}
	}

	return nullptr;
}

void CallInitializeSlot(UObject* Target, const int32 SlotIndex)
{
	if (!Target)
	{
		return;
	}

	UFunction* InitializeSlotFunction = Target->FindFunction(TEXT("InitializeSlot"));
	if (!InitializeSlotFunction)
	{
		InitializeSlotFunction = Target->FindFunction(TEXT("Initialize Slot"));
	}

	if (!InitializeSlotFunction)
	{
		return;
	}

	if (InitializeSlotFunction->NumParms == 1)
	{
		struct FSingleSlotParams
		{
			int32 InSlot = 0;
		};

		FSingleSlotParams Params;
		Params.InSlot = SlotIndex;
		Target->ProcessEvent(InitializeSlotFunction, &Params);
		return;
	}

	struct FAlbumSlotParams
	{
		int32 SlotIndex = 0;
		UTexture2D* Texture = nullptr;
	};

	FAlbumSlotParams Params;
	Params.SlotIndex = SlotIndex;
	Target->ProcessEvent(InitializeSlotFunction, &Params);
}
}

void USW_UIBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind PlayWidgetFade delegate (from BP_UIBase EventGraph)
	PlayWidgetFade.RemoveDynamic(this, &USW_UIBase::OnPlayWidgetFade);
	PlayWidgetFade.AddUniqueDynamic(this, &USW_UIBase::OnPlayWidgetFade);

	// Bind SetAnimationPlaying from interface
	SetAnimationPlaying.RemoveDynamic(this, &USW_UIBase::OnSetAnimationPlaying);
	SetAnimationPlaying.AddUniqueDynamic(this, &USW_UIBase::OnSetAnimationPlaying);

	if (UWidgetAnimation* FadeAnim = GetFadeAnimation())
	{
		FWidgetAnimationDynamicEvent FadeFinishedEvent;
		FadeFinishedEvent.BindDynamic(this, &USW_UIBase::OnFadeAnimationFinished);
		BindToAnimationFinished(FadeAnim, FadeFinishedEvent);
	}

	BlueprintInializeWidget();
	BindCommonReturnButton();
	BindCommonQuitButtons();
	BindCommonPageSwitcherButtons();
	BindCommonSettingButtons();
	BindSaveLoadButtons();
	InitializeLegacyChildSlots();
	RefreshSaveLoadUiState();
}

FReply USW_UIBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bJumpToBacklogOnMouseDown && BacklogIndex != INDEX_NONE)
	{
		if (APlayerController* OwningPlayer = GetOwningPlayer())
		{
			if (ASW_HUD* SWHUD = Cast<ASW_HUD>(OwningPlayer->GetHUD()))
			{
				if (SWHUD->InGameUI && SWHUD->InGameUI->JumpToBacklogIndex(BacklogIndex))
				{
					return FReply::Handled();
				}
			}
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

ASW_ScriptManager* USW_UIBase::GetScriptManager()
{
	if (ScriptManager)
	{
		return ScriptManager;
	}

	AActor* TempActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASW_ScriptManager::StaticClass());
	if (TempActor)
	{
		ScriptManager = Cast<ASW_ScriptManager>(TempActor);
	}
	return ScriptManager;
}

void USW_UIBase::BlueprintInializeWidget_Implementation()
{
}

UWidgetAnimation* USW_UIBase::GetFadeAnimation() const
{
	static const TArray<FName> FadeAnimationNames =
	{
		TEXT("Fade"),
		TEXT("WidgetFade"),
		TEXT("FullScreenBlack")
	};

	return FindWidgetAnimationByName(this, FadeAnimationNames);
}

UMVVM_System* USW_UIBase::GetSystemViewModel() const
{
	const APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		return nullptr;
	}

	const ASW_HUD* SWHUD = Cast<ASW_HUD>(OwningPlayer->GetHUD());
	return SWHUD ? SWHUD->SystemViewModel : nullptr;
}

ASW_HUD* USW_UIBase::GetHUD() const
{
	const APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		return nullptr;
	}

	return Cast<ASW_HUD>(OwningPlayer->GetHUD());
}

UMVVM_LoadScreen* USW_UIBase::FindLoadScreenViewModel(UMVVM_LoadScreen*& AsBPLoadScreenViewModel) const
{
	const ASW_HUD* SWHUD = GetHUD();
	AsBPLoadScreenViewModel = SWHUD ? SWHUD->LoadScreenViewModel : nullptr;
	return AsBPLoadScreenViewModel;
}

UMVVM_System* USW_UIBase::FindSystemViewModel(UMVVM_System*& AsBPSystemViewModel) const
{
	AsBPSystemViewModel = GetSystemViewModel();
	return AsBPSystemViewModel;
}

UMVVM_System* USW_UIBase::FindCGViewModel() const
{
	return GetSystemViewModel();
}

UDataTable* USW_UIBase::GetScriptDataTable(UDataTable*& DataTable)
{
	const ASW_ScriptManager* Mgr = GetScriptManager();
	DataTable = Mgr ? Mgr->GetDataTable() : nullptr;
	return DataTable;
}

int32 USW_UIBase::GetRowDialog(int32& RowDialog)
{
	const ASW_ScriptManager* Mgr = GetScriptManager();
	RowDialog = Mgr ? Mgr->GetRowDialog() : 0;
	return RowDialog;
}

void USW_UIBase::PlayGame()
{
	ASW_ScriptManager* Mgr = GetScriptManager();
	if (!Mgr)
	{
		return;
	}

	Mgr->WidgetState = InGame;
}

void USW_UIBase::SetWidgetPosition_Implementation(UUserWidget* Widget)
{
	if (!Widget)
	{
		return;
	}

	// Default: center in viewport
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot))
	{
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetPosition(FVector2D::ZeroVector);
	}
}

void USW_UIBase::OnRep_ForbiddenInput()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);
	}
}

void USW_UIBase::AnimationStart(UPanelWidget* Target)
{
	UWidgetAnimation* FadeAnim = GetFadeAnimation();
	if (!FadeAnim || !Target)
	{
		return;
	}

	PlayAnimation(FadeAnim);
}

void USW_UIBase::AnimationEnd(UPanelWidget* Target)
{
	// Child classes can override behavior
}

void USW_UIBase::PlayEnterFadeAnimation()
{
	UWidgetAnimation* FadeAnim = GetFadeAnimation();
	bRemoveFromParentAfterFade = false;

	if (!FadeAnim)
	{
		StartFadeUiAnimation();
		EndedFadeUiAnimation();
		return;
	}

	StopAnimation(FadeAnim);
	StartFadeUiAnimation();
	PlayAnimation(FadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

void USW_UIBase::PlayEnterFadeAnimationAndNotify()
{
	bBroadcastFadeFinishedEvent = true;
	PlayingWidgetFade.Broadcast(true);
	PlayEnterFadeAnimation();

	if (!GetFadeAnimation())
	{
		bBroadcastFadeFinishedEvent = false;
		PlayingWidgetFade.Broadcast(false);
	}
}

void USW_UIBase::OnPlayWidgetFade(bool bIsPlayingAnimation)
{
	PlayingWidgetFade.Broadcast(bIsPlayingAnimation);

	UWidgetAnimation* FadeAnim = GetFadeAnimation();
	if (!FadeAnim)
	{
		if (bIsPlayingAnimation)
		{
			RemoveFromParent();
			PlayingWidgetFade.Broadcast(false);
		}
		return;
	}

	if (bIsPlayingAnimation)
	{
		bRemoveFromParentAfterFade = true;
		bBroadcastFadeFinishedEvent = true;
		StartFadeUiAnimation();
		PlayAnimation(FadeAnim);
	}
	else
	{
		bRemoveFromParentAfterFade = false;
		bBroadcastFadeFinishedEvent = false;
		StopAnimation(FadeAnim);
	}
}

UObject* USW_UIBase::FindObjectPropertyOrWidget(UObject* Owner, const FName PropertyOrWidgetName) const
{
	if (!Owner)
	{
		return nullptr;
	}

	if (const FObjectPropertyBase* ObjectProperty = FindFProperty<FObjectPropertyBase>(Owner->GetClass(), PropertyOrWidgetName))
	{
		if (UObject* PropertyValue = ObjectProperty->GetObjectPropertyValue_InContainer(Owner))
		{
			return PropertyValue;
		}
	}

	if (UUserWidget* UserWidget = Cast<UUserWidget>(Owner))
	{
		return UserWidget->GetWidgetFromName(PropertyOrWidgetName);
	}

	return nullptr;
}

UButton* USW_UIBase::FindButtonInNamedWidget(const FName WidgetName, const TArray<FName>& InnerButtonNames) const
{
	UObject* ButtonContainer = FindObjectPropertyOrWidget(const_cast<USW_UIBase*>(this), WidgetName);
	if (UButton* DirectButton = Cast<UButton>(ButtonContainer))
	{
		return DirectButton;
	}

	for (const FName& InnerButtonName : InnerButtonNames)
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(ButtonContainer, InnerButtonName)))
		{
			return Button;
		}
	}

	return nullptr;
}

UButton* USW_UIBase::FindButtonByPath(const TArray<FName>& WidgetPath, const TArray<FName>& ButtonNames) const
{
	UObject* CurrentObject = const_cast<USW_UIBase*>(this);
	for (const FName& PathElement : WidgetPath)
	{
		CurrentObject = FindObjectPropertyOrWidget(CurrentObject, PathElement);
		if (!CurrentObject)
		{
			return nullptr;
		}
	}

	if (UButton* DirectButton = Cast<UButton>(CurrentObject))
	{
		return DirectButton;
	}

	for (const FName& ButtonName : ButtonNames)
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(CurrentObject, ButtonName)))
		{
			return Button;
		}
	}

	return nullptr;
}

void USW_UIBase::BindCommonReturnButton()
{
	static const TArray<FName> InnerButtonNames = {TEXT("BTN_Return"), TEXT("Button")};
	if (UButton* ReturnButton = FindButtonInNamedWidget(TEXT("BP_Return"), InnerButtonNames))
	{
		ReturnButton->OnClicked.RemoveDynamic(this, &USW_UIBase::OnReturnButtonClicked);
		ReturnButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnReturnButtonClicked);
	}
}

void USW_UIBase::BindCommonQuitButtons()
{
	static const TArray<FName> InnerButtonNames = {TEXT("Button"), TEXT("BTN_Yes"), TEXT("BTN_No")};
	if (UButton* YesButton = FindButtonInNamedWidget(TEXT("BP_Yes"), InnerButtonNames))
	{
		YesButton->OnClicked.RemoveDynamic(this, &USW_UIBase::OnConfirmQuitButtonClicked);
		YesButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnConfirmQuitButtonClicked);
	}

	if (UButton* NoButton = FindButtonInNamedWidget(TEXT("BP_No"), InnerButtonNames))
	{
		NoButton->OnClicked.RemoveDynamic(this, &USW_UIBase::OnCancelQuitButtonClicked);
		NoButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnCancelQuitButtonClicked);
	}
}

void USW_UIBase::OnReturnButtonClicked()
{
	if (ASW_ScriptManager* Mgr = GetScriptManager())
	{
		if (Mgr->WidgetState == SettingUI || Mgr->WidgetState == InGameSetting)
		{
			if (UMVVM_System* SystemViewModel = GetSystemViewModel())
			{
				SystemViewModel->SyncSettingSlotFromRuntime();
				SystemViewModel->ApplyCurrentSettings(true);
			}
			else if (UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings())
			{
				GameUserSettings->SaveSettings();
			}
		}
	}

	PlayWidgetFade.Broadcast(true);
}

void USW_UIBase::OnConfirmQuitButtonClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
	}
}

void USW_UIBase::OnCancelQuitButtonClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameAndUI());
	}

	PlayWidgetFade.Broadcast(true);
}

UWidgetSwitcher* USW_UIBase::FindPageSwitcher() const
{
	if (ParentSwitcher)
	{
		return ParentSwitcher;
	}

	static const TArray<FName> PageSwitcherNames =
	{
		TEXT("PageSwitcher_Root"),
		TEXT("PageSwitcher"),
		TEXT("WidgetSwitcher_Root"),
		TEXT("WidgetSwitcher"),
		TEXT("Archive_Root")
	};

	for (const FName& PageSwitcherName : PageSwitcherNames)
	{
		if (UWidgetSwitcher* PageSwitcher = Cast<UWidgetSwitcher>(FindObjectPropertyOrWidget(const_cast<USW_UIBase*>(this), PageSwitcherName)))
		{
			return PageSwitcher;
		}
	}

	return nullptr;
}

bool USW_UIBase::SetPageSwitcherActiveIndex(const int32 NewIndex, const bool bWrap)
{
	UWidgetSwitcher* PageSwitcher = FindPageSwitcher();
	if (!PageSwitcher)
	{
		return false;
	}

	const int32 NumWidgets = PageSwitcher->GetNumWidgets();
	if (NumWidgets <= 0)
	{
		return false;
	}

	int32 TargetIndex = NewIndex;
	if (bWrap)
	{
		TargetIndex = ((NewIndex % NumWidgets) + NumWidgets) % NumWidgets;
	}
	else
	{
		TargetIndex = FMath::Clamp(NewIndex, 0, NumWidgets - 1);
	}

	PageSwitcher->SetActiveWidgetIndex(TargetIndex);
	UpdatePageSwitcherNavigationState();
	return true;
}

bool USW_UIBase::SwitchPageByOffset(const int32 Offset, const bool bWrap)
{
	const UWidgetSwitcher* PageSwitcher = FindPageSwitcher();
	if (!PageSwitcher)
	{
		return false;
	}

	return SetPageSwitcherActiveIndex(PageSwitcher->GetActiveWidgetIndex() + Offset, bWrap);
}

void USW_UIBase::UpdatePageSwitcherNavigationState()
{
	const UWidgetSwitcher* PageSwitcher = FindPageSwitcher();
	const int32 NumWidgets = PageSwitcher ? PageSwitcher->GetNumWidgets() : 0;
	const int32 ActiveIndex = PageSwitcher ? PageSwitcher->GetActiveWidgetIndex() : 0;
	const bool bHasMultiplePages = NumWidgets > 1;

	if (UButton* PreviousButton = FindPreviousPageButton())
	{
		PreviousButton->SetIsEnabled(bHasMultiplePages && (bLoopPageSwitcher || ActiveIndex > 0));
	}

	if (UButton* NextButton = FindNextPageButton())
	{
		NextButton->SetIsEnabled(bHasMultiplePages && (bLoopPageSwitcher || ActiveIndex < NumWidgets - 1));
	}
}

UButton* USW_UIBase::FindNextPageButton() const
{
	static const TArray<FName> NextButtonNames = {TEXT("BTN_Next"), TEXT("Button")};
	return FindButtonInNamedWidget(TEXT("BP_Next"), NextButtonNames);
}

UButton* USW_UIBase::FindPreviousPageButton() const
{
	static const TArray<FName> PreviousButtonNames = {TEXT("BTN_Previous"), TEXT("Button")};
	return FindButtonInNamedWidget(TEXT("BP_Previous"), PreviousButtonNames);
}

void USW_UIBase::BindCommonPageSwitcherButtons()
{
	if (UWidgetSwitcher* PageSwitcher = FindPageSwitcher())
	{
		ParentSwitcher = PageSwitcher;
	}
	else
	{
		return;
	}

	if (UButton* NextButton = FindNextPageButton())
	{
		NextButton->OnClicked.RemoveDynamic(this, &USW_UIBase::OnNextPageButtonClicked);
		NextButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnNextPageButtonClicked);
	}

	if (UButton* PreviousButton = FindPreviousPageButton())
	{
		PreviousButton->OnClicked.RemoveDynamic(this, &USW_UIBase::OnPreviousPageButtonClicked);
		PreviousButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnPreviousPageButtonClicked);
	}

	UpdatePageSwitcherNavigationState();
}

void USW_UIBase::BindCommonSettingButtons()
{
	static const TArray<FName> TextButtonNames = {TEXT("Button")};
	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_Fast"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnTextSpeedFastClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnTextSpeedFastClicked);
	}
	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_Normal"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnTextSpeedNormalClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnTextSpeedNormalClicked);
	}
	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_Slow"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnTextSpeedSlowClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnTextSpeedSlowClicked);
	}
	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_Transient"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnTextSpeedTransientClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnTextSpeedTransientClicked);
	}

	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_Full"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnDisplayFullscreenClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnDisplayFullscreenClicked);
	}
	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_Window"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnDisplayWindowedClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnDisplayWindowedClicked);
	}
	if (UButton* Button = FindButtonInNamedWidget(TEXT("BP_FullWindow"), TextButtonNames))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnDisplayWindowedFullscreenClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnDisplayWindowedFullscreenClicked);
	}

	if (UButton* Button = FindButtonByPath({TEXT("WBP_Value"), TEXT("BP_Plus")}, {TEXT("Button_Plus"), TEXT("Button")}))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnVolumePlusClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnVolumePlusClicked);
	}
	if (UButton* Button = FindButtonByPath({TEXT("WBP_Value"), TEXT("BP_Minus")}, {TEXT("Button_Minus"), TEXT("Button")}))
	{
		Button->OnClicked.RemoveDynamic(this, &USW_UIBase::OnVolumeMinusClicked);
		Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnVolumeMinusClicked);
	}

	if (USlider* Slider = Cast<USlider>(FindObjectPropertyOrWidget(FindObjectPropertyOrWidget(this, TEXT("WBP_Value")), TEXT("VolumeSlider"))))
	{
		Slider->OnValueChanged.RemoveDynamic(this, &USW_UIBase::OnVolumeSliderChanged);
		Slider->OnValueChanged.AddUniqueDynamic(this, &USW_UIBase::OnVolumeSliderChanged);
		Slider->SetValue(GetCurrentVolume());
	}

	if (UComboBoxString* ComboBox = Cast<UComboBoxString>(FindObjectPropertyOrWidget(this, TEXT("ComboBoxString_Resolution"))))
	{
		InitializeResolutionComboBox(ComboBox);
		ComboBox->OnSelectionChanged.RemoveDynamic(this, &USW_UIBase::OnResolutionSelectionChanged);
		ComboBox->OnSelectionChanged.AddUniqueDynamic(this, &USW_UIBase::OnResolutionSelectionChanged);
	}
}

void USW_UIBase::OnNextPageButtonClicked()
{
	SwitchPageByOffset(1, bLoopPageSwitcher);
}

void USW_UIBase::OnPreviousPageButtonClicked()
{
	SwitchPageByOffset(-1, bLoopPageSwitcher);
}

void USW_UIBase::OnTextSpeedFastClicked()
{
	SetTextDisplaySpeed(0.05f);
}

void USW_UIBase::OnTextSpeedNormalClicked()
{
	SetTextDisplaySpeed(0.1f);
}

void USW_UIBase::OnTextSpeedSlowClicked()
{
	SetTextDisplaySpeed(0.2f);
}

void USW_UIBase::OnTextSpeedTransientClicked()
{
	SetTextDisplaySpeed(0.01f);
}

void USW_UIBase::OnDisplayFullscreenClicked()
{
	SetWindowMode(EWindowMode::Fullscreen);
}

void USW_UIBase::OnDisplayWindowedClicked()
{
	SetWindowMode(EWindowMode::Windowed);
}

void USW_UIBase::OnDisplayWindowedFullscreenClicked()
{
	SetWindowMode(EWindowMode::WindowedFullscreen);
}

void USW_UIBase::OnVolumePlusClicked()
{
	AdjustVolume(0.25f);
}

void USW_UIBase::OnVolumeMinusClicked()
{
	AdjustVolume(-0.25f);
}

void USW_UIBase::OnVolumeSliderChanged(const float Value)
{
	SetVolume(Value);
}

void USW_UIBase::OnResolutionSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem.IsEmpty())
	{
		return;
	}

	FString WidthString;
	FString HeightString;
	if (!SelectedItem.Split(TEXT("x"), &WidthString, &HeightString))
	{
		SelectedItem.Split(TEXT("X"), &WidthString, &HeightString);
	}

	const int32 Width = FCString::Atoi(*WidthString.TrimStartAndEnd());
	const int32 Height = FCString::Atoi(*HeightString.TrimStartAndEnd());
	if (Width <= 0 || Height <= 0)
	{
		return;
	}

	if (UMVVM_System* SystemViewModel = GetSystemViewModel())
	{
		SystemViewModel->ScreenResolutionData(FIntPoint(Width, Height));
	}
}

void USW_UIBase::SetTextDisplaySpeed(const float NewSpeed)
{
	if (UMVVM_System* SystemViewModel = GetSystemViewModel())
	{
		SystemViewModel->TextDisplaySpeedData(NewSpeed);
		return;
	}

	if (ASW_ScriptManager* Mgr = GetScriptManager())
	{
		Mgr->SetTextDisplaySpeed(NewSpeed);
	}
}

void USW_UIBase::SetWindowMode(const EWindowMode::Type NewWindowMode)
{
	if (UMVVM_System* SystemViewModel = GetSystemViewModel())
	{
		SystemViewModel->WindowModeData(NewWindowMode);
	}
}

void USW_UIBase::AdjustVolume(const float Delta)
{
	SetVolume(GetCurrentVolume() + Delta);
}

void USW_UIBase::SetVolume(const float NewVolume)
{
	const float ClampedVolume = FMath::Clamp(NewVolume, 0.0f, 1.0f);
	UMVVM_System* SystemViewModel = GetSystemViewModel();
	if (!SystemViewModel)
	{
		return;
	}

	const FString ClassName = GetClass() ? GetClass()->GetName() : FString();
	if (ClassName.Contains(TEXT("SoundEffectVolume")))
	{
		SystemViewModel->SoundEffectVolumeData(ClampedVolume);
	}
	else if (ClassName.Contains(TEXT("ConversationalVoice")))
	{
		SystemViewModel->ConversationalVoiceVolumeData(ClampedVolume);
	}
	else
	{
		SystemViewModel->MasterVolumeData(ClampedVolume);
	}

	if (USlider* Slider = Cast<USlider>(FindObjectPropertyOrWidget(FindObjectPropertyOrWidget(this, TEXT("WBP_Value")), TEXT("VolumeSlider"))))
	{
		Slider->SetValue(ClampedVolume);
	}
}

float USW_UIBase::GetCurrentVolume() const
{
	const UMVVM_System* SystemViewModel = GetSystemViewModel();
	const UMVVM_SettingSlot* SettingSlot = SystemViewModel ? SystemViewModel->GetSettingSlot() : nullptr;
	if (!SettingSlot)
	{
		return 0.5f;
	}

	const FString ClassName = GetClass() ? GetClass()->GetName() : FString();
	if (ClassName.Contains(TEXT("SoundEffectVolume")))
	{
		return SettingSlot->GetSoundEffectVolume();
	}
	if (ClassName.Contains(TEXT("ConversationalVoice")))
	{
		return SettingSlot->GetConversationalVoice();
	}
	return SettingSlot->GetMasterVolume();
}

void USW_UIBase::InitializeResolutionComboBox(UComboBoxString* ComboBox)
{
	UMVVM_System* SystemViewModel = GetSystemViewModel();
	if (!ComboBox || !SystemViewModel)
	{
		return;
	}

	ComboBox->ClearOptions();
	const TArray<FIntPoint> Resolutions = SystemViewModel->GetSupportedSixteenByNineResolutions();
	for (const FIntPoint& Resolution : Resolutions)
	{
		ComboBox->AddOption(FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y));
	}

	const FIntPoint CurrentResolution = SystemViewModel->GetCurrentScreenResolution();
	ComboBox->SetSelectedOption(FString::Printf(TEXT("%d x %d"), CurrentResolution.X, CurrentResolution.Y));
}

void USW_UIBase::InitializeLegacyChildSlots()
{
	for (int32 ContinueSlotIndex = 0; ContinueSlotIndex < 12; ++ContinueSlotIndex)
	{
		const FString SlotSuffix = ContinueSlotIndex == 0 ? FString() : FString::Printf(TEXT("_%d"), ContinueSlotIndex);
		const FName ContinueSlotName(*FString::Printf(TEXT("WBP_LoadSlotWidgetSwitcher%s"), *SlotSuffix));
		CallInitializeSlot(FindObjectPropertyOrWidget(this, ContinueSlotName), ContinueSlotIndex);
	}

	for (int32 AlbumSlotIndex = 0; AlbumSlotIndex < 6; ++AlbumSlotIndex)
	{
		const FString SlotSuffix = AlbumSlotIndex == 0 ? FString() : FString::Printf(TEXT("_%d"), AlbumSlotIndex);
		const FName AlbumSlotName(*FString::Printf(TEXT("BP_AlbumCGSwitcher%s"), *SlotSuffix));
		CallInitializeSlot(FindObjectPropertyOrWidget(this, AlbumSlotName), AlbumSlotIndex);
	}
}

void USW_UIBase::RefreshSaveLoadUiState()
{
	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_ContinueUI")))
	{
		UMVVM_LoadScreen* LoadScreenViewModel = nullptr;
		if (FindLoadScreenViewModel(LoadScreenViewModel) && LoadScreenViewModel)
		{
			if (UTextBlock* NumSlotsText = Cast<UTextBlock>(FindObjectPropertyOrWidget(this, TEXT("TextBlock_NumSlots"))))
			{
				NumSlotsText->SetText(FText::AsNumber(LoadScreenViewModel->GetNumLoadSlots()));
			}

			for (int32 ContinueSlotIndex = 0; ContinueSlotIndex < LoadScreenViewModel->GetNumLoadSlots(); ++ContinueSlotIndex)
			{
				const FString SlotSuffix = ContinueSlotIndex == 0 ? FString() : FString::Printf(TEXT("_%d"), ContinueSlotIndex);
				const FName ContinueSlotName(*FString::Printf(TEXT("WBP_LoadSlotWidgetSwitcher%s"), *SlotSuffix));
				ConfigureContinueSlotState(ContinueSlotIndex, FindObjectPropertyOrWidget(this, ContinueSlotName));
			}
		}

		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("WBP_LoadSlotWidgetSwitcher")))
	{
		ConfigureContinueSlotState(ResolveContinueSlotIndex(), this);
	}
}

void USW_UIBase::OnSetAnimationPlaying(bool IsPlaying)
{
	IsPlayAnimation = IsPlaying;
}

void USW_UIBase::OnFadeAnimationFinished()
{
	EndedFadeUiAnimation();

	const bool bShouldBroadcastFadeFinished = bBroadcastFadeFinishedEvent;
	bBroadcastFadeFinishedEvent = false;

	if (!bRemoveFromParentAfterFade)
	{
		if (bShouldBroadcastFadeFinished)
		{
			PlayingWidgetFade.Broadcast(false);
		}
		return;
	}

	bRemoveFromParentAfterFade = false;
	RemoveFromParent();

	if (bShouldBroadcastFadeFinished)
	{
		PlayingWidgetFade.Broadcast(false);
	}
}

void USW_UIBase::OnOpenInGameSaveUIClicked()
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		SWHUD->OpenContinueUI(ESWSaveLoadMode::SaveFromInGame);
	}
}

void USW_UIBase::OnOpenInGameLoadUIClicked()
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		SWHUD->OpenContinueUI(ESWSaveLoadMode::LoadFromInGame);
	}
}

void USW_UIBase::OnOpenInGameSettingClicked()
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		SWHUD->HandleSettingTransition();
	}
}

void USW_UIBase::OnContinueLoadSlotClicked()
{
	const int32 ContinueSlotIndex = ResolveContinueSlotIndex();
	if (ContinueSlotIndex == INDEX_NONE)
	{
		return;
	}

	if (IsContinueUiSaveMode())
	{
		OpenContinueModal(TEXT("/Game/Blueprints/UI/ContinueUI/BP_Overwrite.BP_Overwrite_C"), ContinueSlotIndex);
		return;
	}

	OpenContinueModal(TEXT("/Game/Blueprints/UI/ContinueUI/BP_AreYouSure.BP_AreYouSure_C"), ContinueSlotIndex);
}

void USW_UIBase::OnContinueSaveSlotClicked()
{
	const int32 ContinueSlotIndex = ResolveContinueSlotIndex();
	if (ContinueSlotIndex == INDEX_NONE || !IsContinueUiSaveMode())
	{
		return;
	}

	SaveCurrentGameToContinueSlot(ContinueSlotIndex);
}

void USW_UIBase::OnContinueConfirmLoadClicked()
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		LoadGameFromContinueSlot(SWHUD->GetPendingContinueSlotIndex());
	}

	CloseContinueModals();
}

void USW_UIBase::OnContinueConfirmOverwriteClicked()
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		SaveCurrentGameToContinueSlot(SWHUD->GetPendingContinueSlotIndex());
	}

	CloseContinueModals();
}

void USW_UIBase::OnContinueModalCancelClicked()
{
	CloseContinueModals();
}

void USW_UIBase::StartFadeUiAnimation_Implementation()
{
	IsPlayAnimation = true;
}

void USW_UIBase::EndedFadeUiAnimation_Implementation()
{
	IsPlayAnimation = false;
}

bool USW_UIBase::AddWidgetToViewportOnce(UUserWidget* Widget, const int32 ZOrder)
{
	if (!Widget)
	{
		return false;
	}

	if (Widget->IsInViewport())
	{
		return false;
	}

	Widget->AddToViewport(ZOrder);
	return true;
}

bool USW_UIBase::RemoveWidgetFromParentIfValid(UWidget* Widget)
{
	if (!Widget)
	{
		return false;
	}

	Widget->RemoveFromParent();
	return true;
}

void USW_UIBase::BindSaveLoadButtons()
{
	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_SaveGame")))
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(this, TEXT("BTN_SaveGame"))))
		{
			Button->OnClicked.RemoveAll(this);
			Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnOpenInGameSaveUIClicked);
		}
		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_LoadGame")))
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(this, TEXT("BTN_LoadGame"))))
		{
			Button->OnClicked.RemoveAll(this);
			Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnOpenInGameLoadUIClicked);
		}
		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_InGameSetting")))
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(this, TEXT("BTN_Setting"))))
		{
			Button->OnClicked.RemoveAll(this);
			Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnOpenInGameSettingClicked);
		}
		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_LoadSlot")))
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(this, TEXT("BTN_Load"))))
		{
			Button->OnClicked.RemoveAll(this);
			Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnContinueLoadSlotClicked);
		}
		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_SaveSlot")))
	{
		if (UButton* Button = Cast<UButton>(FindObjectPropertyOrWidget(this, TEXT("BTN_SaveSlot"))))
		{
			Button->OnClicked.RemoveAll(this);
			Button->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnContinueSaveSlotClicked);
		}
		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_AreYouSure")))
	{
		if (UButton* PlayButton = FindButtonInNamedWidget(TEXT("BP_Play"), {TEXT("BTN_Play"), TEXT("Button")}))
		{
			PlayButton->OnClicked.RemoveAll(this);
			PlayButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnContinueConfirmLoadClicked);
		}

		if (UButton* CancelButton = FindButtonInNamedWidget(TEXT("BP_Cancel"), {TEXT("BTN_Cancel"), TEXT("Button")}))
		{
			CancelButton->OnClicked.RemoveAll(this);
			CancelButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnContinueModalCancelClicked);
		}
		return;
	}

	if (IsGeneratedFromWidgetBlueprint(TEXT("BP_Overwrite")))
	{
		if (UButton* YesButton = FindButtonInNamedWidget(TEXT("BP_YesOverwrite"), {TEXT("BTN_Yes"), TEXT("Button")}))
		{
			YesButton->OnClicked.RemoveAll(this);
			YesButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnContinueConfirmOverwriteClicked);
		}

		if (UButton* NoButton = FindButtonInNamedWidget(TEXT("BP_NoOverwrite"), {TEXT("BTN_No"), TEXT("Button")}))
		{
			NoButton->OnClicked.RemoveAll(this);
			NoButton->OnClicked.AddUniqueDynamic(this, &USW_UIBase::OnContinueModalCancelClicked);
		}
	}
}

bool USW_UIBase::IsGeneratedFromWidgetBlueprint(const TCHAR* WidgetBlueprintName) const
{
	if (!WidgetBlueprintName || !GetClass())
	{
		return false;
	}

	const FString ClassName = GetClass()->GetName();
	const FString ClassPath = GetClass()->GetPathName();
	const FString GeneratedClassName = FString::Printf(TEXT("%s_C"), WidgetBlueprintName);
	return ClassName.Contains(GeneratedClassName) || ClassPath.Contains(WidgetBlueprintName);
}

void USW_UIBase::ConfigureContinueSlotState(const int32 ContinueSlotIndex, UObject* SlotSwitcherWidget) const
{
	if (!SlotSwitcherWidget || ContinueSlotIndex == INDEX_NONE)
	{
		return;
	}

	SetSlotIndexOnWidget(SlotSwitcherWidget, ContinueSlotIndex);

	if (USW_UIBase* SaveSlotWidget = Cast<USW_UIBase>(FindObjectPropertyOrWidget(SlotSwitcherWidget, TEXT("BP_SaveSlot"))))
	{
		SetSlotIndexOnWidget(SaveSlotWidget, ContinueSlotIndex);
		if (UButton* SaveButton = Cast<UButton>(FindObjectPropertyOrWidget(SaveSlotWidget, TEXT("BTN_SaveSlot"))))
		{
			SaveButton->SetIsEnabled(IsContinueUiSaveMode());
		}
	}

	if (USW_UIBase* LoadSlotWidget = Cast<USW_UIBase>(FindObjectPropertyOrWidget(SlotSwitcherWidget, TEXT("BP_LoadSlot"))))
	{
		SetSlotIndexOnWidget(LoadSlotWidget, ContinueSlotIndex);
		if (UButton* LoadButton = Cast<UButton>(FindObjectPropertyOrWidget(LoadSlotWidget, TEXT("BTN_Load"))))
		{
			LoadButton->SetIsEnabled(GetContinueSlotStatus(ContinueSlotIndex) == Load);
		}
	}

	if (UWidgetSwitcher* SlotSwitcher = FindContinueSlotSwitcher(SlotSwitcherWidget))
	{
		SlotSwitcher->SetActiveWidgetIndex(static_cast<int32>(GetContinueSlotStatus(ContinueSlotIndex)));
	}
}

void USW_UIBase::SetSlotIndexOnWidget(UObject* Widget, const int32 InSlotIndex) const
{
	if (USW_UIBase* UIWidget = Cast<USW_UIBase>(Widget))
	{
		UIWidget->SlotIndex = InSlotIndex;
	}
}

int32 USW_UIBase::ResolveContinueSlotIndex() const
{
	if (SlotIndex >= 0)
	{
		return SlotIndex;
	}

	for (UObject* OuterObject = GetOuter(); OuterObject; OuterObject = OuterObject->GetOuter())
	{
		if (const USW_UIBase* OuterWidget = Cast<USW_UIBase>(OuterObject))
		{
			if (OuterWidget->SlotIndex >= 0)
			{
				return OuterWidget->SlotIndex;
			}
		}
	}

	return INDEX_NONE;
}

UWidgetSwitcher* USW_UIBase::FindContinueSlotSwitcher(UObject* SlotSwitcherWidget) const
{
	if (UWidgetSwitcher* DirectSwitcher = Cast<UWidgetSwitcher>(SlotSwitcherWidget))
	{
		return DirectSwitcher;
	}

	static const TArray<FName> CandidateNames =
	{
		TEXT("Archive_Root"),
		TEXT("WidgetSwitcher"),
		TEXT("WidgetSwitcher_Root")
	};

	for (const FName& CandidateName : CandidateNames)
	{
		if (UWidgetSwitcher* Switcher = Cast<UWidgetSwitcher>(FindObjectPropertyOrWidget(SlotSwitcherWidget, CandidateName)))
		{
			return Switcher;
		}
	}

	return nullptr;
}

ESaveSlotStatus USW_UIBase::GetContinueSlotStatus(const int32 ContinueSlotIndex) const
{
	UMVVM_LoadScreen* LoadScreenViewModel = nullptr;
	if (!FindLoadScreenViewModel(LoadScreenViewModel) || !LoadScreenViewModel)
	{
		return Save;
	}

	if (UMVVM_LoadSlot* LoadSlotViewModel = LoadScreenViewModel->GetLoadSlotViewModelByIndex(ContinueSlotIndex))
	{
		return LoadSlotViewModel->SlotStatus;
	}

	return Save;
}

void USW_UIBase::OpenContinueModal(const TCHAR* WidgetClassPath, const int32 ContinueSlotIndex)
{
	ASW_HUD* SWHUD = GetHUD();
	APlayerController* OwningPlayer = GetOwningPlayer();
	if (!SWHUD || !OwningPlayer || !WidgetClassPath)
	{
		return;
	}

	CloseContinueModals();
	SWHUD->SetPendingContinueSlotIndex(ContinueSlotIndex);

	if (UClass* ModalClass = LoadClass<USW_UIBase>(nullptr, WidgetClassPath))
	{
		if (USW_UIBase* ModalWidget = CreateWidget<USW_UIBase>(OwningPlayer, ModalClass))
		{
			ModalWidget->AddToViewport(3);
			OwningPlayer->SetInputMode(FInputModeUIOnly());
		}
	}
}

void USW_UIBase::CloseContinueModals()
{
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UUserWidget::StaticClass(), false);
	for (UUserWidget* Widget : Widgets)
	{
		if (!Widget)
		{
			continue;
		}

		const FString WidgetClassName = Widget->GetClass()->GetName();
		if (WidgetClassName.Contains(TEXT("BP_AreYouSure_C")) || WidgetClassName.Contains(TEXT("BP_Overwrite_C")))
		{
			Widget->RemoveFromParent();
		}
	}

	if (APlayerController* OwningPlayer = GetOwningPlayer())
	{
		OwningPlayer->SetInputMode(FInputModeGameAndUI());
	}
}

bool USW_UIBase::IsContinueUiSaveMode() const
{
	const ASW_HUD* SWHUD = GetHUD();
	return SWHUD && SWHUD->IsContinueUISaveMode();
}

bool USW_UIBase::IsContinueUiOpenedFromInGame() const
{
	const ASW_HUD* SWHUD = GetHUD();
	return SWHUD && SWHUD->IsContinueUIOpenedFromInGame();
}

bool USW_UIBase::SaveCurrentGameToContinueSlot(const int32 ContinueSlotIndex)
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		const bool bSaved = SWHUD->SaveCurrentGameToSlot(ContinueSlotIndex);
		if (bSaved)
		{
			RefreshSaveLoadUiState();
		}
		return bSaved;
	}

	return false;
}

bool USW_UIBase::LoadGameFromContinueSlot(const int32 ContinueSlotIndex)
{
	if (ASW_HUD* SWHUD = GetHUD())
	{
		const bool bLoaded = SWHUD->LoadGameFromSlot(ContinueSlotIndex);
		if (bLoaded)
		{
			RefreshSaveLoadUiState();
		}
		return bLoaded;
	}

	return false;
}
