// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

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
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot>LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 index);

	UFUNCTION(BlueprintCallable)
	void SaveSlotButtonPressed(int32 Slot,UDataTable *EnterDataTable);

	UFUNCTION(BlueprintCallable)
	void ReadSlotButtonPressed(int32 Slot);

	void LoadData();

	void SetNumLoadSlots(int32 InNumLoadSlots);

	int GetNumLoadSlots() const {return NumLoadSlots;}
private:
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
