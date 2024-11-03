// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_SettingSlot.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UMVVM_SettingSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()


public:
	
	void SetTextDisplaySpeed(float InTextDisplaySpeed);
	void SetMasterVolume(float InMasterVolume);
	void SetSoundEffectVolume(float InSoundEffect);
	

	float GetTextDisplaySpeed() const {return  TextDisplaySpeed;}
	float GetMasterVolume() const {return  MasterVolume;}
	float GetSoundEffectVolume() const {return  SoundEffectVolume;}
	
private:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float TextDisplaySpeed = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float MasterVolume = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float SoundEffectVolume = 0.5f;
};
