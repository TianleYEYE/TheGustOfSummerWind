#pragma once

#include "CoreMinimal.h"
#include "Game/SW_ScriptManager.h"
#include "UI/SW_UIBase.h"
#include "SW_QuitGameUI.generated.h"

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_QuitGameUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Fade;

	UPROPERTY()
	TObjectPtr<ASW_ScriptManager> AudioPlayer;

	FTimerHandle PlayFade;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ConfirmQuit();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void CancelQuit();

	UFUNCTION()
	void OnFadeFinished();

protected:
	virtual UWidgetAnimation* GetFadeAnimation() const override { return Fade; }
	void QuitGameNow();

private:
	bool bQuitAfterFade = false;
};
