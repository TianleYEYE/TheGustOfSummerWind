// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStruct\DialogStruct.h"
#include "GlobalVariablesManager.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UGlobalVariablesManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int rowDialog=0;

	
	
	FDialogStruct *GetDialogStruct(UDataTable *dialogDataTable,int row);
};
