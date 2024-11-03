// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_SettingSlot.h"


void UMVVM_SettingSlot::SetTextDisplaySpeed(float InTextDisplaySpeed)
{
	UE_MVVM_SET_PROPERTY_VALUE(TextDisplaySpeed,InTextDisplaySpeed);
}

void UMVVM_SettingSlot::SetMasterVolume(float InMasterVolume)
{
	UE_MVVM_SET_PROPERTY_VALUE(MasterVolume,InMasterVolume);
}

void UMVVM_SettingSlot::SetSoundEffectVolume(float InSoundEffect)
{
	UE_MVVM_SET_PROPERTY_VALUE(SoundEffectVolume,InSoundEffect);
}

