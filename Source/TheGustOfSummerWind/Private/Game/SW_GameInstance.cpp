// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SW_GameInstance.h"

void USW_GameInstance::Init()
{
	Super::Init();
}



FDialogStruct *USW_GameInstance::SetDialogStruct(UDataTable *dialogDataTable)
{
	MaxRowDialog=GetMaxDialogIndex();
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Cyan,FString::Printf(TEXT("MaxRowDialog :%d"),MaxRowDialog));
	
	if (rowDialog<=MaxRowDialog)
	{
		DialogStruct=dialogDataTable->FindRow<FDialogStruct>(dialogDataTable->GetRowNames()[rowDialog],TEXT(""));
		return DialogStruct;
	}
	return DialogStruct;
}

FDialogStruct* USW_GameInstance::GetDialogStruct()
{
	return DialogStruct;
}

int USW_GameInstance::SetDialogIndex(int index)
{
	rowDialog=index;
	return rowDialog;
}

int USW_GameInstance::GetMaxDialogIndex()
{
	if (DataTable)
	{
		return DataTable->GetRowMap().Num();
	}
	return  0;
}
