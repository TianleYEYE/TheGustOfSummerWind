// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheGustOfSummerWind\Public\DataStruct\DialogStruct.h"
#include "SW_UIBase.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_UIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int rowDialog=0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	TObjectPtr<UDataTable>dialogDataTable;
	
	UFUNCTION(BlueprintType,BlueprintCallable)
	FDialogStruct GetDialogStruct(int row);
	
	FTimerHandle UpdataTextHandle;
	
	FDialogStruct dialogStruct;
	FDialogStruct *DialogRow;
};
