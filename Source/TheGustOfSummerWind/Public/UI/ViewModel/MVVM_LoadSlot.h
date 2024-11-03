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
	
	
	void InitializeSlot() ;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	void SetLoadSlotName(FString InLoadSlotName);
	void SetBackgroundMusic(USoundBase *InBackgroundMusic);
	void SetDataTable(UDataTable *InDataTable);
	void SetChapterName(FString InChapterName);
	void SetRowDialog(int32 InRowDialog);
	void SetDateTime(FDateTime InDateTime);
	
	FString GetLoadSlotName() const {return LoadSlotName; }
	USoundBase *GetBackgroundMusic() const {return  BackgroundMusic;}
	UDataTable *GetDataTable() const {return DataTable; }
	FString GetChapterName() const {return ChapterName; }
	int32 GetRowDialog() const {return RowDialog; }
	FDateTime GetDateTime() const { return DateTime;}

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString LoadSlotName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	TObjectPtr<USoundBase> BackgroundMusic;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDataTable>DataTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString ChapterName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	int32 RowDialog;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FDateTime DateTime;
	
};
