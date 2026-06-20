// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_UIBase.h"

#include "Components/Widget.h"
#include "Game/SW_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGameUI/SW_InGameUI.h"

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
		ScriptManager= Cast<ASW_ScriptManager>(TempActor);
	}
	return ScriptManager;
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
