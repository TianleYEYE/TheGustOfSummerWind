// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/QuitGameUI/SW_QuitGameUI.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void USW_QuitGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Fade)
	{
		FWidgetAnimationDynamicEvent FadeFinished;
		FadeFinished.BindDynamic(this, &USW_QuitGameUI::OnFadeFinished);
		BindToAnimationFinished(Fade, FadeFinished);
	}

	static const TArray<FName> InnerButtonNames = {TEXT("Button"), TEXT("BTN_Yes"), TEXT("BTN_No")};
	if (UButton* YesButton = FindButtonInNamedWidget(TEXT("BP_Yes"), InnerButtonNames))
	{
		YesButton->OnClicked.RemoveDynamic(this, &USW_QuitGameUI::ConfirmQuit);
		YesButton->OnClicked.AddUniqueDynamic(this, &USW_QuitGameUI::ConfirmQuit);
	}

	if (UButton* NoButton = FindButtonInNamedWidget(TEXT("BP_No"), InnerButtonNames))
	{
		NoButton->OnClicked.RemoveDynamic(this, &USW_QuitGameUI::CancelQuit);
		NoButton->OnClicked.AddUniqueDynamic(this, &USW_QuitGameUI::CancelQuit);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeUIOnly());
	}
}

void USW_QuitGameUI::ConfirmQuit()
{
	bQuitAfterFade = true;

	if (Fade)
	{
		PlayAnimation(Fade);
	}
	else
	{
		QuitGameNow();
	}
}

void USW_QuitGameUI::CancelQuit()
{
	bQuitAfterFade = false;

	if (Fade)
	{
		PlayAnimation(Fade);
	}
	else
	{
		RemoveFromParent();
	}
}

void USW_QuitGameUI::OnFadeFinished()
{
	if (bQuitAfterFade)
	{
		QuitGameNow();
		return;
	}

	if (IsInViewport())
	{
		RemoveFromParent();
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameAndUI());
	}
}

void USW_QuitGameUI::QuitGameNow()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
	}
}
