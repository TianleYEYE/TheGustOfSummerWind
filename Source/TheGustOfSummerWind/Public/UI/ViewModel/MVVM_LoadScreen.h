// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayButtonPressed,int32,EnterRowDialog,UDataTable*,EnterDatatable,USoundBase*,EnterBackgroundMusic);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

	
public:
	void InitializeLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	UPROPERTY(BlueprintAssignable)
	FPlayButtonPressed PlayButton;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot>LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 index);

	UFUNCTION(BlueprintCallable)
	void SaveSlotButtonPressed(int32 Slot,UDataTable *EnterDataTable,int32 EnterRowDialog,USoundBase *EnterBackgroundMusic);
	

	UFUNCTION(BlueprintCallable)
	void ReadSlotButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed(int32 Slot);

	void SetNumLoadSlots(int32 InNumLoadSlots);

	int GetNumLoadSlots() const {return NumLoadSlots;}
private:
	UPROPERTY()
	TMap<int32,UMVVM_LoadSlot*>LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot>LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot>LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot>LoadSlot_2;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot>LoadSlot_3;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot>LoadSlot_4;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot>LoadSlot_5;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	int32 NumLoadSlots;
};
