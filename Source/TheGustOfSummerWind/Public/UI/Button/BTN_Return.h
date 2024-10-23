// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTN_ButtonBase.h"
#include "../../../../../../UE_5.3/Engine/Source/Runtime/UMG/Public/Components/Button.h"
#include "BTN_Return.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustReturn)
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Return : public UBTN_ButtonBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Return;

	FEntrustReturn Return;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void DelegatedAgent();
};
