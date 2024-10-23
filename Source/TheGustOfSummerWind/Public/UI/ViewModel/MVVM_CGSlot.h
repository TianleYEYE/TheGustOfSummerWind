// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_CGSlot.generated.h"

UENUM(BlueprintType)
enum EAlbumStatus
{
	NotUnlocked,
	Unlocked
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCGLock,int32,WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UMVVM_CGSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeSlot() ;

	UPROPERTY(BlueprintAssignable)
	FSetCGLock SetCGLock;
	
	UPROPERTY()
	TEnumAsByte<EAlbumStatus> SlotStatus;
	
	void SetCGSlotName(FString InCGSlotName);

	FString GetCGSlotName() const {return CGSlotName; }
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"))
	FString CGSlotName;

	
};
