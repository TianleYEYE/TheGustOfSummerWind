// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CircularThrobber.h"
#include "Components/RichTextBlock.h"
#include "DataStruct\DialogStruct.h"
#include "UI/SW_UIBase.h"
#include "SW_DialogBox.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_DialogBox : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<URichTextBlock>RichTextBlock_Dialogue;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>DialogFade;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>DialogShallow;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCircularThrobber>Throbber_1;

	TArray<FString>MessageCharactArray;

	FDialogStruct *DialogRow;
	
	void SetDialogStructAndCurrentIndex(FDialogStruct * ParentsDialogStruct,int Index);
	
	void SetDialog();
	
	void UpdateText();
	
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle UpdataTextHandle;

	int CurrentIndex = 0;
};
