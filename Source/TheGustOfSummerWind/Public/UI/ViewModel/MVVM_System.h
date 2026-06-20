// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_CGSlot.h"
#include "MVVM_SettingSlot.h"
#include "MVVM_System.generated.h"

class USoundClass;
class USoundMix;

USTRUCT(BlueprintType)
struct THEGUSTOFSUMMERWIND_API FCGSlotData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TEnumAsByte<EAlbumStatus> AlbumStatus = NotUnlocked;
};

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UMVVM_System : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_CGSlot>CGSlotViewModelClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_SettingSlot>SettingSlotViewModelClass;
	
	UFUNCTION(BlueprintPure)
	UMVVM_CGSlot* GetCGSlotViewModelByIndex(int32 index);

	UFUNCTION(BlueprintCallable)
	void CGSlotUnlock(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void TextDisplaySpeedData(float InTextDisplaySpeed);

	UFUNCTION(BlueprintCallable)
	void MasterVolumeData(float InMasterVolume);

	UFUNCTION(BlueprintCallable)
	void SoundEffectVolumeData(float InSoundEffectVolume);

	UFUNCTION(BlueprintCallable)
	void ConversationalVoiceVolumeData(float InConversationalVoice);

	UFUNCTION(BlueprintCallable)
	void ScreenResolutionData(FIntPoint InScreenResolution);

	UFUNCTION(BlueprintCallable)
	void WindowModeData(TEnumAsByte<EWindowMode::Type> InWindowMode);

	UFUNCTION(BlueprintCallable)
	void ApplySoundClassVolume(USoundMix* InSoundMix, USoundClass* InSoundClass, float InVolume, bool bApplyToChildren = true);

	UFUNCTION(BlueprintCallable)
	void ApplyCurrentSettings(bool bSaveAfterApply = true);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentSettings();

	UFUNCTION(BlueprintCallable)
	void SyncSettingSlotFromRuntime();

	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetSupportedSixteenByNineResolutions() const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetCurrentScreenResolution() const;

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EWindowMode::Type> GetCurrentWindowMode() const;
	
	UFUNCTION(BlueprintCallable)
	void LoadData();
	
	void SetNumLoadSlots(int32 InNumLoadSlots);

	static bool IsSixteenByNineResolution(const FIntPoint& Resolution);

	int GetNumLoadSlots() const {return NumLoadSlots;}
	
	TMap<int32,UMVVM_CGSlot*>GetCGSlots() { return CGSlots; }

	UMVVM_SettingSlot *GetSettingSlot () const {return SettingSlot;}
private:
	void ApplyVideoSettings() const;
	void ApplyAudioSettings();
	void ApplyRuntimeSettings() const;
	void SaveSettingSlot();
	class ASW_GameMode* GetSWGameMode() const;
	class ASW_ScriptManager* GetScriptManager() const;

	UPROPERTY()
	TMap<int32,UMVVM_CGSlot*>CGSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_SettingSlot>SettingSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundMix> GlobalSoundMix;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundClass> SoundEffectSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundClass> ConversationalVoiceSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Album")
	int32 DefaultCGSlotCount = 6;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	int32 NumLoadSlots;
};
