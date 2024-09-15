#pragma once

#include "CoreMinimal.h"
#include "Game/SW_ScriptManager.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInQuitGame/BTN_No.h"
#include "UI/Button/BTN_ButtonInQuitGame/BTN_Yes.h"
#include "SW_QuitGameUI.generated.h"



UCLASS()
class THEGUSTOFSUMMERWIND_API USW_QuitGameUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_Yes>BP_Yes;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_No>BP_No;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Fade;

	UPROPERTY()
	TObjectPtr<ASW_ScriptManager>AudioPlayer;

	FTimerHandle PlayFade;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void ReturnGame();

	void QuitGameImplement();

	void ReturnGameImlement();
};
