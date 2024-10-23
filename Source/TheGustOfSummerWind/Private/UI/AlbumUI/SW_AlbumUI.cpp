// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AlbumUI/SW_AlbumUI.h"

#include "Game/SW_HUD.h"
#include "Kismet/GameplayStatics.h"


void USW_AlbumUI::NativeConstruct()
{
	Super::NativeConstruct();

	PageSwitcher->SetActiveWidgetIndex(0);
	
	BP_Next->Next.AddUObject(this,&USW_AlbumUI::SwitchToNextPage_2);
	BP_Previous->Previous.AddUObject(this,&USW_AlbumUI::SwitchToNextPage_1);
	BP_Return->AlbumReturn.AddUObject(this,&USW_AlbumUI::OnReturnButtonClicked);
}

void USW_AlbumUI::SwitchToNextPage_2()
{
	PageSwitcher->SetActiveWidgetIndex(1);
}

void USW_AlbumUI::SwitchToNextPage_1()
{
	PageSwitcher->SetActiveWidgetIndex(0);
}

void USW_AlbumUI::SetParentSwitcher(UWidgetSwitcher* InSwitcher)
{
	ParentSwitcher=InSwitcher;
}

void USW_AlbumUI::OnReturnButtonClicked()
{
	if (ParentSwitcher)
	{
		ParentSwitcher->SetActiveWidgetIndex(0); // Set the index to the desired widget
	}
}


