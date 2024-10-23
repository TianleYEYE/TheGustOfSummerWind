// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_CGSlot.h"

void UMVVM_CGSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetCGLock.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_CGSlot::SetCGSlotName(FString InCGSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(CGSlotName,InCGSlotName);
}

