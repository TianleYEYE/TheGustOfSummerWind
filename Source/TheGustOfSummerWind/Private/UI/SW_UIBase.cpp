// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_UIBase.h"
FDialogStruct  USW_UIBase::GetDialogStruct(int row)
{
	 
	DialogRow =dialogDataTable->FindRow<FDialogStruct>(dialogDataTable->GetRowNames()[row],TEXT("1"));
	dialogStruct=*DialogRow;
	return dialogStruct;
}