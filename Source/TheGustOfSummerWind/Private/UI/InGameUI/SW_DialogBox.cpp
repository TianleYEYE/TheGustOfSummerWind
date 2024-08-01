// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUI/SW_DialogBox.h"

#include "Components/Throbber.h"

void USW_DialogBox::NativeConstruct()
{
	Super::NativeConstruct();

	if (Throbber_1)
	{
		Throbber_1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USW_DialogBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateVisibility();
}


void USW_DialogBox::UpdateVisibility()
{


	if (GetWorld()->GetTimerManager().IsTimerActive(UpdataTextHandle))
	{
		Throbber_1->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Throbber_1->SetVisibility(ESlateVisibility::Hidden);
	}

	// 确保持续更新
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]
		{
			UpdateVisibility();
		});
	}
}

