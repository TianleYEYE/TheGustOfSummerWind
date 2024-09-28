// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/SW_SaveGame.h"
#include "MVVM_LoadSlot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex,int32,WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton,bool,bEnable);

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;
	
	void InitializeSlot() ;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	void SetLoadSlotName(FString InLoadSlotName);
	void SetDataTable(UDataTable *InDataTable);
	void SetChapterName(FString InChapterName);
	
	FString GetLoadSlotName() const {return LoadSlotName; }
	UDataTable *GetDataTable() const {return DataTable; }
	FString GetChapterName() const {return ChapterName; }

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString LoadSlotName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDataTable>DataTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString ChapterName;

	
};
