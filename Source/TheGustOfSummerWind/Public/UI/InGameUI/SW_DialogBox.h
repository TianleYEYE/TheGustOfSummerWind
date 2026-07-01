// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_UIBase.h"
#include "SW_DialogBox.generated.h"

class UWidget;
class UBorder;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateText, FText, Text);

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_AlphaTextBox : public USW_UIBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Text Reveal")
	bool bIsReading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget> Throbber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Widgets")
	TArray<TObjectPtr<UWidget>> BP_AlphaTextSingle;
};

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_DialogBox : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetDialogText(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void SetSpeakerName(const FText& SpeakerName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog")
	bool IsDialogTextRevealing() const;

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void CompleteDialogTextReveal();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialog")
	int32 GetCurrentDialogTextLength() const;

	UPROPERTY(BlueprintAssignable, Category = "MyDelegate")
	FUpdateText UpdateDialogText;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle UpdataTextHandle;

private:
	USW_UIBase* GetAlphaTextBox() const;
	UTextBlock* GetSpeakerNameTextBlock() const;
	UBorder* GetSpeakerNameBackground() const;

	UPROPERTY(Transient)
	FText CurrentDialogText;

	UPROPERTY(Transient)
	FText CurrentSpeakerName;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> CachedSpeakerNameTextBlock;

	UPROPERTY(Transient)
	TObjectPtr<UBorder> CachedSpeakerNameBackground;
};
