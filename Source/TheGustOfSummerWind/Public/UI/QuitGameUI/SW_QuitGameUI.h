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

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Fade;

	UPROPERTY()
	TObjectPtr<ASW_ScriptManager>AudioPlayer;

	FTimerHandle PlayFade;
	
	virtual void NativeConstruct() override;

};
