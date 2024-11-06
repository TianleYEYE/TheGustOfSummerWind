// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_CGSlot.h"
#include "MVVM_SettingSlot.h"
#include "MVVM_System.generated.h"

USTRUCT(BlueprintType)
struct THEGUSTOFSUMMERWIND_API FCGSlotData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TEnumAsByte<EAlbumStatus> AlbumStatus;
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
	void LoadData();
	
	void SetNumLoadSlots(int32 InNumLoadSlots);

	int GetNumLoadSlots() const {return NumLoadSlots;}
	
	TMap<int32,UMVVM_CGSlot*>GetCGSlots() { return CGSlots; }

	UMVVM_SettingSlot *GetSettingSlot () const {return SettingSlot;}
private:
	UPROPERTY()
	TMap<int32,UMVVM_CGSlot*>CGSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_2;
	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_3;
	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_4;
	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_5;
	UPROPERTY()
	TObjectPtr<UMVVM_CGSlot>CGSlot_6;

	UPROPERTY()
	TObjectPtr<UMVVM_SettingSlot>SettingSlot;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	int32 NumLoadSlots;
};
