// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
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
	void SetConversationalVoice(float InConversationalVoice);
	void SetScreenResolution(const FIntPoint& InScreenResolution);
	void SetWindowMode(EWindowMode::Type InWindowMode);

	float GetTextDisplaySpeed() const {return  TextDisplaySpeed;}
	float GetMasterVolume() const {return  MasterVolume;}
	float GetSoundEffectVolume() const {return  SoundEffectVolume;}
	float GetConversationalVoice() const {return  ConversationalVoice;}
	FIntPoint GetScreenResolution() const {return ScreenResolution;}
	EWindowMode::Type GetWindowMode() const {return WindowMode.GetValue();}
	
private:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float TextDisplaySpeed = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float MasterVolume = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float SoundEffectVolume = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	float ConversationalVoice = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FIntPoint ScreenResolution = FIntPoint(1920, 1080);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::WindowedFullscreen;
};
