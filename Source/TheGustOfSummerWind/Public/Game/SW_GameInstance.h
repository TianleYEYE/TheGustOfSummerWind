// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "DataStruct\DialogStruct.h"


#include "SW_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int rowDialog=0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	TObjectPtr<UDataTable>DataTable;

	int MaxRowDialog=0;
	
	FDialogStruct *SetDialogStruct(UDataTable *dialogDataTable);
	FDialogStruct *GetDialogStruct();
	FDialogStruct *DialogStruct;
	
	int SetDialogIndex(int index);
	int GetMaxDialogIndex();

	void Init() override;
};
