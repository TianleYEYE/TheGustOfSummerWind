// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/SW_UIBase.h"
#include "OpeningLogosHUD.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API AOpeningLogosHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USW_UIBase>OpeningLogosClass;

	
	UPROPERTY(BlueprintReadOnly,Category="Widget")
	TObjectPtr<USW_UIBase>OpeningLogos;

	UPROPERTY(EditAnywhere, Category = "Opening Logos")
	FName MainMenuLevelName = TEXT("MasterMenu");

	UPROPERTY(EditAnywhere, Category = "Opening Logos", meta = (ClampMin = "0.0"))
	float OpeningLogoDuration = 2.5f;

	virtual void BeginPlay() override;

private:
	FTimerHandle OpenMainMenuTimerHandle;

	UFUNCTION()
	void OpenMainMenu();
};
