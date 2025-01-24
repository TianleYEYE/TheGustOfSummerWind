// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "UI/SW_UIBase.h"
#include "SW_CharacterPortraits.generated.h"


/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_CharacterPortraits : public USW_UIBase
{
	GENERATED_BODY()

public:
	
	// UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	// TObjectPtr<UWidgetAnimation>FaceChange;
	// UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	// TObjectPtr<UWidgetAnimation>AppearCharacter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UImage>VerticalPainting;

	

	FWidgetAnimationDynamicEvent FaceChangeEvent;
	FDialogStruct *DialogStruct;
	FDialogStruct *PreviousDialogStruct;

	void NativeConstruct() override;
	void SetCharacterPortraits();
};