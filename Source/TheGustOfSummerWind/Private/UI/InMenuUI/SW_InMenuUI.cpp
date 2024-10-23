// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InMenuUI/SW_InMenuUI.h"

#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void USW_InMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	TurnBlack.BindDynamic(this,&USW_InMenuUI::FlashingOver);
	BindToAnimationFinished(FullScreenBlack,TurnBlack);
	BP_ButtonOfInMenu->BP_NewGame->NewGame.AddUObject(this,&USW_InMenuUI::PressBTN_NewGame);
	BP_ButtonOfInMenu->BP_Album->Album.AddUObject(this,&USW_InMenuUI::PressBTN_Album);
	BP_ButtonOfInMenu->BP_Continue->Continue.AddDynamic(this,&USW_InMenuUI::PressBTN_Continue);
	BP_ButtonOfInMenu->BP_Setting->Setting.AddUObject(this,&USW_InMenuUI::PressBTN_Setting);
}

void USW_InMenuUI::PressBTN_NewGame()
{
	//播放按钮点击声音
	PlayATapSound();

	//播放按钮闪烁动画
	BP_ButtonOfInMenu->BP_NewGame->PlayAnimation(BP_ButtonOfInMenu->BP_NewGame->flashing);

	//设置黑屏的显示层级为1，遮挡按钮且防止触发其他键
	SetScreenBlackZOrder(3);
	//播放黑屏动画
	PlayAnimation(FullScreenBlack);
}

void USW_InMenuUI::PressBTN_Album()
{
	PlayATapSound();

	//播放按钮闪烁动画
	BP_ButtonOfInMenu->BP_Album->PlayAnimation(BP_ButtonOfInMenu->BP_Album->flashing);
	
	//设置黑屏的显示层级为1，遮挡按钮且防止触发其他键
	SetScreenBlackZOrder(3);
	//播放黑屏动画
	PlayAnimation(FullScreenBlack);
}

void USW_InMenuUI::PressBTN_Continue()
{
	
	PlayATapSound();

	//播放按钮闪烁动画
	BP_ButtonOfInMenu->BP_Continue->PlayAnimation(BP_ButtonOfInMenu->BP_Continue->flashing);
	
	//设置黑屏的显示层级为1，遮挡按钮且防止触发其他键
	SetScreenBlackZOrder(3);
	//播放黑屏动画
	PlayAnimation(FullScreenBlack);
}

void USW_InMenuUI::PressBTN_Setting()
{
	
	PlayATapSound();

	//播放按钮闪烁动画
	BP_ButtonOfInMenu->BP_Setting->PlayAnimation(BP_ButtonOfInMenu->BP_Setting->flashing);
	
	//设置黑屏的显示层级为1，遮挡按钮且防止触发其他键
	SetScreenBlackZOrder(3);
	//播放黑屏动画
	PlayAnimation(FullScreenBlack);
}

void USW_InMenuUI::FlashingOver()
{
	UCanvasPanelSlot *slot=Cast<UCanvasPanelSlot>(ScreenBlack->Slot);
	slot->SetZOrder(0);
}

void USW_InMenuUI::PlayATapSound()
{
	USoundBase*click=LoadObject<USoundBase>(this,TEXT("/Game/Assets/Audio/Click/switch_button_push_small_06_Cue.switch_button_push_small_06_Cue"));
	UGameplayStatics::PlaySound2D(this,click);
}


void USW_InMenuUI::SetScreenBlackZOrder(int32 NewZOrder)
{
	UCanvasPanelSlot *slot=Cast<UCanvasPanelSlot>(ScreenBlack->Slot);
	slot->SetZOrder(NewZOrder);
}