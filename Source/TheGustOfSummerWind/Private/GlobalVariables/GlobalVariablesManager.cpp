// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalVariables/GlobalVariablesManager.h"

FDialogStruct *UGlobalVariablesManager::GetDialogStruct(UDataTable *dialogDataTable,int row)
{
	return dialogDataTable->FindRow<FDialogStruct>(dialogDataTable->GetRowNames()[row],TEXT(""));;
}
